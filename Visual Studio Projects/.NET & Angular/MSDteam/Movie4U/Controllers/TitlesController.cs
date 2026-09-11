using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using System.Threading.Tasks;

namespace Movie4U.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class TitlesController : ControllerBase
    {
        private readonly ITitlesManager manager;

        public TitlesController(ITitlesManager manager)
        {
            this.manager = manager;
        }

        [HttpGet("GetAllTitles")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetAllTitlesAsync()
        {
            var titles = await manager.GetAllAsync();

            if (titles.Count == 0)
                return NotFound("There are no titles stored in the database");

            return Ok(titles);
        }

        [HttpGet("GetTitleById/{netflix_id}")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetTitleByIdAsync([FromRoute] string netflix_id)
        {
            var title = await manager.GetOneByIdAsync(netflix_id);

            if (title == null)
                return NotFound("There is no title with the given id stored in the database");

            return Ok(title);
        }

        [HttpPost]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> CreateTitleAsync([FromBody] TitleModelParameter titleModelParam)
        {
            await manager.Create(titleModelParam);
            return Ok();
        }

        [HttpPut]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> UpdateTitleAsync([FromBody] TitleModelParameter titleModelParam)
        {
            await manager.Update(titleModelParam);
            return Ok();
        }

        [HttpDelete]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> DeleteTitleAsync([FromBody] string netflix_id)
        {
            await manager.Delete(netflix_id);
            return Ok();
        }

    }
}