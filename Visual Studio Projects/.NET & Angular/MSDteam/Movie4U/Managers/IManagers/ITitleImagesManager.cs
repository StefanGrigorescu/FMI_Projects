using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface ITitleImagesManager
    {
        Task<List<TitleImageModel>> GetAllAsync();

        Task<List<TitleImageModel>> GetAllByNetflixIdAsync(string netflixId);

        Task<TitleImageModel> GetOneByIdAsync(string url);

        Task Update(TitleImageModel titleImageModel);

        Task Create(TitleImageModel titleImageModel);

        Task Delete(string url);
    }
}
