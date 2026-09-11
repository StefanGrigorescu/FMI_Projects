using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Repositories.IRepositories
{
    public interface IWatcherGenresRepository: IGenericRepository<WatcherGenre, WatcherGenreModel>
    {
        Task<List<WatcherGenreModel>> GetAllByWatcherIdAsync(string watcher_name);

    }
}
