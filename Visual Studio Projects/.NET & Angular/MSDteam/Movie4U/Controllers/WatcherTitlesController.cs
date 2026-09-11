using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using System.Threading.Tasks;

namespace Movie4U.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class WatcherTitlesController : ControllerBase
    {
        private readonly IWatcherTitlesManager manager;

        public WatcherTitlesController(IWatcherTitlesManager manager)
        {
            this.manager = manager;
        }

        [HttpGet("GetAllWatcherTitles")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetAllWatcherTitlesAsync()
        {
            var watcherTitles = await manager.GetAllAsync();

            if (watcherTitles.Count == 0)
                return NotFound("There are no watcher titles stored in the database");

            return Ok(watcherTitles);
        }

        [HttpGet("GetWatcherTitleById/{watcher_name}/{netflix_id}")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetWatcherTitleByIdAsync([FromRoute] string watcher_name, string netflix_id)
        {
            var watcherTitle = await manager.GetOneByIdAsync(watcher_name, netflix_id);

            if (watcherTitle == null)
                return NotFound("There is no watcher title with the given id stored in the database");

            return Ok(watcherTitle);
        }

        [HttpPost]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> CreateWatcherTitleAsync([FromBody] WatcherTitleModelParameter watcherTitleModelParam)
        {
            await manager.Create(watcherTitleModelParam);
            return Ok();
        }

        [HttpPut]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> UpdateWatcherTitleAsync([FromBody] WatcherTitleModelParameter watcherTitleModelParam)
        {
            await manager.Update(watcherTitleModelParam);
            return Ok();
        }

        [HttpDelete]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> DeleteWatcherTitleAsync([FromBody] string watcher_name, string netflix_id)
        {
            await manager.Delete(watcher_name, netflix_id);
            return Ok();
        }

    }
}