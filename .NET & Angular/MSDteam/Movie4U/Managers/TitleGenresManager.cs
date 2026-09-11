using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories.IRepositories;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class TitleGenresManager: ITitleGenresManager
    {
        private readonly ITitleGenresRepository repo;
        private readonly IGenresManager genresManager;

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleGenresManager(ITitleGenresRepository repo, IGenresManager genresManager)
        {
            this.repo = repo;
            this.genresManager = genresManager;
        }


        public async Task<List<TitleGenreModel>> GetAllAsync()
        {
            return await repo.GetAllAsync();
        }

        public async Task<List<TitleGenreModel>> GetAllByNetflixIdAsync(string netflixId)
        {
            return await repo.GetAllByNetflixIdAsync(netflixId);
        }

        public async Task<List<GenreModel>> GetAllGenresByNetflixIdAsync(string netflixId)
        {
            var titleGenres = await GetAllByNetflixIdAsync(netflixId);

            var genres = new List<GenreModel>();
            foreach(var titleGenre in titleGenres)
            {
                var genre = await genresManager.GetOneByIdAsync(titleGenre.genre_id);
                genres.Add(genre);
            }

            return genres;
        }

        public async Task<TitleGenreModel> GetOneByIdAsync(int genre_id, string netflix_id)
        {
            return await repo.GetOneByIdAsync(genre_id, netflix_id);
        }

        public async Task Create(TitleGenreModel titleGenreModel)
        {
            TitleGenre newTitleGenre = new TitleGenre(titleGenreModel);

            await repo.InsertAsync(newTitleGenre);
        }

        public async Task Update(TitleGenreModel titleGenreModel)
        {
            TitleGenre updateTitleGenre = await repo.GetOneDbByIdAsync(titleGenreModel.genre_id, titleGenreModel.netflix_id);
            updateTitleGenre.Copy(titleGenreModel);

            await repo.UpdateAsync(updateTitleGenre);
        }

        public async Task Delete(int genre_id, string netflix_id)
        {
            TitleGenre delTitleGenre = await repo.GetOneDbByIdAsync(genre_id, netflix_id);

            if (delTitleGenre != null)
                await repo.DeleteAsync(delTitleGenre);
        }

    }
}
