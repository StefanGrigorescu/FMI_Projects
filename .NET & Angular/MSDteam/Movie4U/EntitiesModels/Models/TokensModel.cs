
namespace Movie4U.EntitiesModels.Models
{
    public class TokensModel
    {
        public string accessToken { get; set; }
        public string refreshToken { get; set; }

        public void Copy(TokensModel source)
        {
            accessToken = source.accessToken;
            refreshToken = source.refreshToken;
        }
    }
}
