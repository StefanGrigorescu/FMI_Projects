using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface IGenresManager
    {
        Task<List<GenreModel>> GetAllAsync();

        Task<GenreModel> GetOneByIdAsync(int genre_id);

        Task Update(GenreModel genreModel);

        Task Create(GenreModel genreModel);

        Task Delete(int genre_id);
    }
}
