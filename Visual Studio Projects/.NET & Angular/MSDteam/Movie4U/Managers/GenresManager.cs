using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories.IRepositories;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class GenresManager : IGenresManager
    {
        private readonly IGenresRepository repo;

        /**<summary>
         * Constructor.
         * </summary>*/
        public GenresManager(IGenresRepository repo)
        {
            this.repo = repo;
        }


        public async Task<List<GenreModel>> GetAllAsync()
        {
            return await repo.GetAllAsync();
        }

        public async Task<GenreModel> GetOneByIdAsync(int genre_id)
        {
            return await repo.GetOneByIdAsync(genre_id);
        }

        public async Task Create(GenreModel genreModel)
        {
            Genre newGenre = new Genre(genreModel);

            await repo.InsertAsync(newGenre);
        }

        public async Task Update(GenreModel genreModel)
        {
            Genre updateGenre = await repo.GetOneDbByIdAsync(genreModel.genre_id);
            updateGenre.Copy(genreModel);

            await repo.UpdateAsync(updateGenre);
        }

        public async Task Delete(int genre_id)
        {
            Genre delGenre = await repo.GetOneDbByIdAsync(genre_id);

            if (delGenre != null)
                await repo.DeleteAsync(delGenre);
        }

    }
}
