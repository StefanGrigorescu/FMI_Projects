using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories.IRepositories;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class WatcherTitlesManager: IWatcherTitlesManager
    {
        private readonly IWatcherTitlesRepository repo;
        private readonly ITitlesManager titlesManager;

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitlesManager(IWatcherTitlesRepository repo, ITitlesManager titlesManager)
        {
            this.repo = repo;
            this.titlesManager = titlesManager;
        }


        private async Task<bool> FillModelsLists(WatcherTitleModel watcherTitleModel)
        {
            watcherTitleModel.titleModel = await titlesManager.GetOneByIdAsync(watcherTitleModel.watcher_name);

            return true;
        }

        public async Task<List<WatcherTitleModel>> GetAllAsync()
        {
            var watcherTitleModels = await repo.GetAllAsync();

            foreach (var watcherTitleModel in watcherTitleModels)
                await FillModelsLists(watcherTitleModel);

            return watcherTitleModels;
        }

        public async Task<List<WatcherTitleModel>> GetAllByWatcherIdAsync(string watcher_name)
        {
            var watcherTitleModels = await repo.GetAllByWatcherIdAsync(watcher_name);

            foreach (var watcherTitleModel in watcherTitleModels)
                await FillModelsLists(watcherTitleModel);

            return watcherTitleModels;
        }

        public async Task<WatcherTitleModel> GetOneByIdAsync(string watcher_name, string netflix_id)
        {
            var watcherTitleModel = await repo.GetOneByIdAsync(watcher_name, netflix_id);
            await FillModelsLists(watcherTitleModel);

            return watcherTitleModel;
        }

        public async Task Create(WatcherTitleModelParameter watcherTitleModelParam)
        {
            WatcherTitle newWatcherTitle = new WatcherTitle(watcherTitleModelParam);

            await repo.InsertAsync(newWatcherTitle);
        }

        public async Task Update(WatcherTitleModelParameter watcherTitleModelParam)
        {
            WatcherTitle updateWatcherTitle = await repo.GetOneDbByIdAsync(watcherTitleModelParam.watcher_name, watcherTitleModelParam.netflix_id);
            updateWatcherTitle.Copy(watcherTitleModelParam);

            await repo.UpdateAsync(updateWatcherTitle);
        }

        public async Task Delete(string watcher_name, string netflix_id)
        {
            WatcherTitle delWatcherTitle = await repo.GetOneDbByIdAsync(watcher_name, netflix_id);

            if (delWatcherTitle != null)
                await repo.DeleteAsync(delWatcherTitle);
        }

    }
}