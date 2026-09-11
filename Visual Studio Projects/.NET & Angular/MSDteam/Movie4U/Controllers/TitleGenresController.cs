using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using System.Threading.Tasks;

namespace Movie4U.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class TitleGenresController : ControllerBase
    {
        private readonly ITitleGenresManager manager;

        public TitleGenresController(ITitleGenresManager manager)
        {
            this.manager = manager;
        }

        [HttpGet("GetAllTitleGenres")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetAllTitleGenresAsync()
        {
            var titleGenres = await manager.GetAllAsync();

            if (titleGenres.Count == 0)
                return NotFound("There are no title genres stored in the database");

            return Ok(titleGenres);
        }

        [HttpGet("GetTitleGenreById/{genre_id}/{netflix_id}")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetTitleGenreByIdAsync([FromRoute] int genre_id, string netflix_id)
        {
            var titleGenre = await manager.GetOneByIdAsync(genre_id, netflix_id);

            if (titleGenre == null)
                return NotFound("There is no title genre with the given id stored in the database");

            return Ok(titleGenre);
        }

        [HttpPost]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> CreateTitleGenreAsync([FromBody] TitleGenreModel titleGenreModel)
        {
            await manager.Create(titleGenreModel);
            return Ok();
        }

        [HttpPut]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> UpdateTitleGenreAsync([FromBody] TitleGenreModel titleGenreModel)
        {
            await manager.Update(titleGenreModel);
            return Ok();
        }

        [HttpDelete]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> DeleteTitleGenreAsync([FromBody] int genre_id, string netflix_id)
        {
            await manager.Delete(genre_id, netflix_id);
            return Ok();
        }

    }
}