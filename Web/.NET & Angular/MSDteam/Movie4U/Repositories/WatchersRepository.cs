using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Repositories.IRepositories;

namespace Movie4U.Repositories
{
    public class WatchersRepository: GenericRepository<Watcher, WatcherModel>, IWatchersRepository
    {
        /**<summary>
         * Constructor.
         * </summary>*/
        public WatchersRepository(Movie4UContext db) : base(db) { }

    }
}
