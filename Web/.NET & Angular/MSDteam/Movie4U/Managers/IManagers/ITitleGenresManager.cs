using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface ITitleGenresManager
    {
        Task<List<TitleGenreModel>> GetAllAsync();

        Task<List<TitleGenreModel>> GetAllByNetflixIdAsync(string netflixId);

        Task<List<GenreModel>> GetAllGenresByNetflixIdAsync(string netflixId);

        Task<TitleGenreModel> GetOneByIdAsync(int genre_id, string netflix_id);

        Task Update(TitleGenreModel titleGenreModel);

        Task Create(TitleGenreModel titleGenreModel);

        Task Delete(int genre_id, string netflix_id);
    }
}
