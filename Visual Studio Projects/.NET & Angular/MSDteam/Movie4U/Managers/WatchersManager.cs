using Microsoft.AspNetCore.Identity;
using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories.IRepositories;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class WatchersManager: IWatchersManager
    {
        private readonly IWatchersRepository repo;
        private readonly UserManager<User> userManager;
        private readonly ITokensManager tokensManager;

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatchersManager(IWatchersRepository repo, UserManager<User> userManager, ITokensManager tokensManager)
        {
            this.repo = repo;
            this.userManager = userManager;
            this.tokensManager = tokensManager;
        }


        public async Task<List<WatcherModel>> GetAllAsync()
        {
            return await repo.GetAllAsync();
        }

        public async Task<WatcherModel> GetOneByIdAsync(string name)
        {
            return await repo.GetOneByIdAsync(name);
        }

        public async Task Create(string watcherName, string UserId)
        {
            Watcher newWatcher = new Watcher
            {
                watcher_name = watcherName,
                register_date = DateTime.Now,
                userId = UserId
            };

            await repo.InsertAsync(newWatcher);
        }

        public async Task UpdadeRefreshTokenAndExpTime(string watcherName, string refreshToken, DateTime refTokExpTime)
        {
            Watcher watcher = await repo.GetOneDbByIdAsync(watcherName);
            
            watcher.refreshToken = refreshToken;
            watcher.refreshTokenExpiryTime = refTokExpTime;

            await repo.UpdateAsync(watcher);
        }

        public async Task<TokensModel> UpdateRefreshToken(WatcherModel watcher)
        {
            User user =
                await
                userManager
                .FindByNameAsync(watcher.watcher_name);
            
            var accessToken =
                tokensManager
                .GenerateAccessToken(user)
                .Result;
            var refreshToken =
                tokensManager
                .GenerateRefreshToken();

            Watcher dbWatcher = await repo.GetOneDbByIdAsync(watcher.watcher_name);
            dbWatcher.refreshToken = refreshToken;

            await repo.UpdateAsync(dbWatcher);

            TokensModel tokensModel = new TokensModel
            {
                accessToken = accessToken,
                refreshToken = refreshToken
            };
            return tokensModel;
        }

        public async Task Delete(string name)
        {
            Watcher delWatcher = await repo.GetOneDbByIdAsync(name);

            if (delWatcher != null)
                await repo.DeleteAsync(delWatcher);
        }

    }
}
