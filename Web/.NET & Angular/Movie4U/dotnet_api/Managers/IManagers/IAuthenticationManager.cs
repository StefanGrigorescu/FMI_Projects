using Movie4U.EntitiesModels.Models;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface IAuthenticationManager
    {
        Task<bool> Signup(RegisterModel registerModel);
        Task<TokensModel> Login(LoginModel loginModel);
    }
}
