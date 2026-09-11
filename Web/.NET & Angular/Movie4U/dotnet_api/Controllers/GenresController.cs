using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using System.Threading.Tasks;

namespace Movie4U.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class GenresController : ControllerBase
    {
        private readonly IGenresManager manager;

        public GenresController(IGenresManager manager)
        {
            this.manager = manager;
        }

        [HttpGet("GetAllGenres")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetAllGenresAsync()
        {
            var genres = await manager.GetAllAsync();

            if (genres.Count == 0)
                return NotFound("There are no genres stored in the database");

            return Ok(genres);
        }

        [HttpGet("GetGenreById/{genre_id}")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetGenreByIdAsync([FromRoute] int genre_id)
        {
            var genre = await manager.GetOneByIdAsync(genre_id);

            if (genre == null)
                return NotFound("There is no genre with the given id stored in the database");

            return Ok(genre);
        }

        [HttpPost]
        [Authorize(Policy ="AdminPolicy")]
        public async Task<IActionResult> CreateGenreAsync([FromBody] GenreModel genreModel)
        {
            await manager.Create(genreModel);
            return Ok();
         }

        [HttpPut]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> UpdateGenreAsync([FromBody] GenreModel genreModel)
        {
            await manager.Update(genreModel);
            return Ok();
        }

        [HttpDelete]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> DeleteGenreAsync([FromBody] int genre_id)
        {
            await manager.Delete(genre_id);
            return Ok();
        }

    }
}
