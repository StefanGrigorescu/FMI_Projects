using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using System.Threading.Tasks;

namespace Movie4U.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class CountriesController : ControllerBase
    {
        private readonly ICountriesManager manager;

        public CountriesController(ICountriesManager manager)
        {
            this.manager = manager;
        }

        [HttpGet("GetAllCountries")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetAllCountriesAsync()
        {
            var countries = await manager.GetAllAsync();

            if (countries.Count == 0)
                return NotFound("There are no countries stored in the database");

            return Ok(countries);
        }

        [HttpGet("GetCountryByCode/{country_code}")]
        [Authorize(Policy = "BasicUserPolicy")]
        public async Task<IActionResult> GetCountryByCodeAsync([FromRoute] string country_code)
        {
            var country = await manager.GetOneByIdAsync(country_code);

            if (country == null)
                return NotFound("There is no country with the given code stored in the database");

            return Ok(country);
        }

        [HttpPost]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> CreateCountryAsync([FromBody] CountryModel countryModel)
        {
            await manager.Create(countryModel);
            return Ok();
        }

        [HttpPut]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> UpdateCountryAsync([FromBody] CountryModel countryModel)
        {
            await manager.Update(countryModel);
            return Ok();
        }


        [HttpDelete]
        [Authorize(Policy = "AdminPolicy")]
        public async Task<IActionResult> DeleteCountryAsync([FromBody] string country_code)
        {
            await manager.Delete(country_code);
            return Ok();
        }

    }
}
