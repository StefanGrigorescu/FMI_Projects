using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Repositories.IRepositories
{
    public interface IWatcherTitlesRepository: IGenericRepository<WatcherTitle, WatcherTitleModel>
    {
        Task<List<WatcherTitleModel>> GetAllByWatcherIdAsync(string watcher_name);

    }
}
