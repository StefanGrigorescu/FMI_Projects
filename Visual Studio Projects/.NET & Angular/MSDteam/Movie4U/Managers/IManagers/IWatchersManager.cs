using Movie4U.EntitiesModels.Models;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface IWatchersManager
    {
        Task<List<WatcherModel>> GetAllAsync();

        Task<WatcherModel> GetOneByIdAsync(string name);
        
        Task Create(string watcherName, string UserId);

        Task UpdadeRefreshTokenAndExpTime(string watcherName, string refreshToken, DateTime refTokExpTime);

        Task<TokensModel> UpdateRefreshToken(WatcherModel watcher);
        
        Task Delete(string name);
    }
}
