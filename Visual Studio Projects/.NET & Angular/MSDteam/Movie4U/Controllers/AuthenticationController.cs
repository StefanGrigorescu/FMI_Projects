using Microsoft.AspNetCore.Mvc;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Utilities;
using System.Threading.Tasks;

namespace Movie4U.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class AuthenticationController : ControllerBase
    {
        private readonly IAuthenticationManager manager;

        /**<summary>
         * Constructor.
         * </summary>*/
        public AuthenticationController(IAuthenticationManager manager)
        {
            this.manager = manager;
        }


        [HttpPost("SignupUser")]
        public async Task<IActionResult> SignupUserAsync([FromBody] RegisterModel registerModel)
        {
            registerModel.role = "BasicUser";

            if (registerModel == null  || NullCheckerUtility.HasNulls(registerModel))
                return BadRequest("Invalid client request");
            
            var result = await manager.Signup(registerModel);

            if(result)
                return Ok("Signup succedeed");

            return BadRequest("Failed to signup");
        }

        [HttpPost("Signup")]
        // TODO: Add this in production: [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> SignupAsync([FromBody] RegisterModel registerModel)
        {
            if (registerModel == null  || NullCheckerUtility.HasNulls(registerModel))
                return BadRequest("Invalid client request");

            var result = await manager.Signup(registerModel);

            if (result)
                return Ok("Signup succedeed");

            return BadRequest("Failed to signup");
        }

        [HttpPost("Login")]
        public async Task<IActionResult> LoginAsync([FromBody] LoginModel loginModel)
        {
            if (loginModel == null || NullCheckerUtility.HasNulls(loginModel))
                return BadRequest("Invalid client request");

            var tokens = await manager.Login(loginModel);

            if (tokens != null)
                return Ok(tokens);

            return BadRequest("Failed to login");
        }
    }
}
