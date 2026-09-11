using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories.IRepositories;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class WatcherGenresManager: IWatcherGenresManager
    {
        private readonly IWatcherGenresRepository repo;
        private readonly IGenresManager genresManager;

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherGenresManager(IWatcherGenresRepository repo, IGenresManager genresManager)
        {
            this.repo = repo;
            this.genresManager = genresManager;
        }


        private async Task<bool> FillModelsLists(WatcherGenreModel watcherGenreModel)
        {
            watcherGenreModel.genreModel = await genresManager.GetOneByIdAsync(watcherGenreModel.genre_id);

            return true;
        }

        public async Task<List<WatcherGenreModel>> GetAllAsync()
        {
            var watcherGenreModels = await repo.GetAllAsync();

            foreach (var watcherGenreModel in watcherGenreModels)
                await FillModelsLists(watcherGenreModel);

            return watcherGenreModels;
        }

        public async Task<List<WatcherGenreModel>> GetAllByWatcherIdAsync(string watcher_name)
        {
            var watcherGenreModels = await repo.GetAllByWatcherIdAsync(watcher_name);

            foreach (var watcherGenreModel in watcherGenreModels)
                await FillModelsLists(watcherGenreModel);

            return watcherGenreModels;
        }

        public async Task<WatcherGenreModel> GetOneByIdAsync(string watcher_name, int genre_id)
        {
            var watcherGenreModel = await repo.GetOneByIdAsync(watcher_name, genre_id);
            await FillModelsLists(watcherGenreModel);

            return watcherGenreModel;
        }

        public async Task Create(WatcherGenreModelParameter watcherGenreModelParameter)
        {
            WatcherGenre newWatcherGenre = new WatcherGenre(watcherGenreModelParameter);

            await repo.InsertAsync(newWatcherGenre);
        }

        public async Task Update(WatcherGenreModelParameter watcherGenreModelParam)
        {
            WatcherGenre updateWatcherGenre = await repo.GetOneDbByIdAsync(watcherGenreModelParam.watcher_name, watcherGenreModelParam.genre_id);
            updateWatcherGenre.Copy(watcherGenreModelParam);

            await repo.UpdateAsync(updateWatcherGenre);
        }

        public async Task Delete(string watcher_name, int genre_id)
        {
            WatcherGenre WatcherGenre = await repo.GetOneDbByIdAsync(watcher_name, genre_id);

            if (WatcherGenre != null)
                await repo.DeleteAsync(WatcherGenre);
        }

    }
}