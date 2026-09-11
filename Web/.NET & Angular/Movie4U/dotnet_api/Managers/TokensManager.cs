using Microsoft.Extensions.Configuration;
using Microsoft.AspNetCore.Identity;
using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.IdentityModel.Tokens;
using System.Text;
using System.Security.Cryptography;
using Movie4U.Managers.IManagers;
using Movie4U.EntitiesModels.Entities;

namespace Movie4U.Managers
{
    public class TokensManager : ITokensManager
    {
        private readonly IConfiguration configuration;
        private readonly UserManager<User> userManager;

        public TokensManager(IConfiguration configuration, UserManager<User> userManager)
        {
            this.configuration = configuration;
            this.userManager = userManager;
        }


        public async Task<string> GenerateAccessToken(User user)
        {
            var roles = await userManager.GetRolesAsync(user);
            var claims = new List<Claim>();

            foreach (var role in roles)
                claims.Add(new Claim(ClaimTypes.Role, role));

            claims.Add(new Claim(ClaimTypes.NameIdentifier, user.Id));
            claims.Add(new Claim(ClaimTypes.Name, user.UserName));

            var secretKey =
                configuration
                .GetSection("Jwt")
                .GetSection("SecretKey")
                .Get<string>();

            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(secretKey));
            var credentials = new SigningCredentials(key, SecurityAlgorithms.HmacSha512Signature);

            var tokenDescription = new SecurityTokenDescriptor
            {
                Subject = new ClaimsIdentity(claims),
                Expires = DateTime.Now.AddMinutes(15),
                SigningCredentials = credentials
            };
            var tokenHandler = new JwtSecurityTokenHandler();
            var accessToken = tokenHandler.CreateToken(tokenDescription);

            return tokenHandler.WriteToken(accessToken);
        }


        public string GenerateRefreshToken()
        {
            var randomNumber = new byte[32];
            using (var rng = RandomNumberGenerator.Create())
            {
                rng.GetBytes(randomNumber);
                return Convert.ToBase64String(randomNumber);
            }
        }


        public ClaimsPrincipal GetPrincipalFromExpiredToken(string token)
        {
            var secretKey =
                configuration
                .GetSection("Jwt")
                .GetSection("SecretKey")
                .Get<string>();

            var tokenValidationParams = new TokenValidationParameters
            {
                ValidateAudience = false,
                ValidateIssuer = false,
                ValidateIssuerSigningKey = true,
                IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(secretKey)),
                ValidateLifetime = false    // we know token should be already expired
            };

            var tokenHandler = new JwtSecurityTokenHandler();
            SecurityToken securityToken;
            var principal = tokenHandler.ValidateToken(token, tokenValidationParams, out securityToken);
                            // out modifier: argument passed by refference; may be unitialized before it is passed; may have its value modified

            var jwtSecurityToken = securityToken as JwtSecurityToken;
                            // as operator: cast between compatible refference types or Nullable types; shortcut from using "is" operator; returns null when fails to cast
            if (jwtSecurityToken == null || !jwtSecurityToken.Header.Alg.Equals(SecurityAlgorithms.HmacSha512, StringComparison.InvariantCultureIgnoreCase))
                throw new SecurityTokenException("Invalid token");
                            // algorithm check

            return principal;
        }


        public string ExtractUserName(string tokenHeader)
        {
            var handler = new JwtSecurityTokenHandler();
            var accessToken =  handler.ReadJwtToken(tokenHeader.Split(' ')[1]); // token format is:    Bearer + space + accessToken
            var claims = accessToken.Payload.Claims.ToList();

            var watcherName = "";
            foreach (var claim in claims)
                if (claim.Type == "unique_name")
                {
                    watcherName = claim.Value;
                    break;
                }

            return watcherName;
        }

    }
}
