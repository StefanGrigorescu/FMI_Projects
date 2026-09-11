using Microsoft.AspNetCore.Identity;
using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using System;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class AuthenticationManager : IAuthenticationManager
    {
        private readonly UserManager<User> userManager;
        private readonly SignInManager<User> signInManager;
        private readonly ITokensManager tokensManager;
        private readonly IWatchersManager watchersManager;

        public AuthenticationManager(UserManager<User> userManager, SignInManager<User> signInManager, ITokensManager tokensManager, IWatchersManager watchersManager)
        {
            this.userManager = userManager;
            this.signInManager = signInManager;
            this.tokensManager = tokensManager;

            this.watchersManager = watchersManager;
        }

        public async Task<bool> Signup(RegisterModel registerModel)
        {
            User user = new User
            {
                Email = registerModel.email,
                UserName = registerModel.userName
            };

            var result = await userManager.CreateAsync(user, registerModel.password);

            if (result.Succeeded)
            {
                await userManager.AddToRoleAsync(user, registerModel.role);
                await watchersManager.Create(user.UserName, user.Id);

                return true;
            }
            
            return false;
        }


        public async Task<TokensModel> Login(LoginModel loginModel)
        {
            User user;
            if(loginModel.emailOrName.Contains("@"))
                user = await userManager.FindByEmailAsync(loginModel.emailOrName);
            else
                user = await userManager.FindByNameAsync(loginModel.emailOrName);

            if (user != null)
            {
                var result = await signInManager.CheckPasswordSignInAsync(user, loginModel.password, false);
                if (result.Succeeded)
                {
                    var accessToken = await tokensManager.GenerateAccessToken(user);
                    var refreshToken = tokensManager.GenerateRefreshToken();
                    DateTime refTokExpTime = DateTime.Now.AddDays(7);

                    await watchersManager.UpdadeRefreshTokenAndExpTime(user.UserName, refreshToken, refTokExpTime);

                    WatcherModel watcher = await watchersManager.GetOneByIdAsync(user.UserName);

                    return new TokensModel
                    {
                        accessToken = accessToken,
                        refreshToken = refreshToken
                    };
                }
            }

            return null;
        }
    }
}
