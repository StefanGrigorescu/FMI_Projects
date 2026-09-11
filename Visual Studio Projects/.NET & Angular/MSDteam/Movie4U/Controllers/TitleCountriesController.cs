using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using System.Threading.Tasks;

namespace Movie4U.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class TitleCountriesController : ControllerBase
    {
        private readonly ITitleCountriesManager manager;

        public TitleCountriesController(ITitleCountriesManager manager)
        {
            this.manager = manager;
        }

        [HttpGet("GetAllTitleCountries")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetAllTitleCountriesAsync()
        {
            var titleCountries = await manager.GetAllAsync();

            if (titleCountries.Count == 0)
                return NotFound("There are no title countries stored in the database");

            return Ok(titleCountries);
        }

        [HttpGet("GetTitleCountryById/{country_code}/{netflix_id}")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetTitleCountryByIdAsync([FromRoute] string country_code, string netflix_id)
        {
            var titleCountry = await manager.GetOneByIdAsync(country_code, netflix_id);

            if (titleCountry == null)
                return NotFound("There is no title country with the given id stored in the database");

            return Ok(titleCountry);
        }

        [HttpPost]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> CreateTitleCountryAsync([FromBody] TitleCountryModel titleCountryModel)
        {
            await manager.Create(titleCountryModel);
            return Ok();
        }

        [HttpPut]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> UpdateTitleCountryAsync([FromBody] TitleCountryModel titleCountryModel)
        {
            await manager.Update(titleCountryModel);
            return Ok();
        }

        [HttpDelete]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> DeleteTitleCountryAsync([FromBody] string country_code, string netflix_id)
        {
            await manager.Delete(country_code, netflix_id);
            return Ok();
        }

    }
}
