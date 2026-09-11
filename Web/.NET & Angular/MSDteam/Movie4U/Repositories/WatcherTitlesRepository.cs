using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Repositories.IRepositories;
using Movie4U.Utilities;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Movie4U.Repositories
{
    public class WatcherTitlesRepository: GenericRepository<WatcherTitle, WatcherTitleModel>, IWatcherTitlesRepository
    {
        /**<summary>
        * Constructor.
        * </summary>>*/
        public WatcherTitlesRepository(Movie4UContext db) : base(db) { }


        public async Task<List<WatcherTitleModel>> GetAllByWatcherIdAsync(string watcher_name)
        {
            var allDb = await GetAllDbAsync();
            var watcherTitles =
                allDb
                .Where(wt => wt.watcher_name == watcher_name)
                .ToList();

            return CastUtility.ToModelsList<WatcherTitle, WatcherTitleModel>(watcherTitles);
        }
        
    }
}
