using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface ITitleCountriesManager
    {
        Task<List<TitleCountryModel>> GetAllAsync();

        Task<List<TitleCountryModel>> GetAllByNetflixIdAsync(string netflixId);

        Task<List<CountryModel>> GetAllCountriesByNetflixIdAsync(string netflixId);

        Task<TitleCountryModel> GetOneByIdAsync(string country_code, string netflix_id);

        Task Update(TitleCountryModel titleCountryModel);

        Task Create(TitleCountryModel titleCountryModel);

        Task Delete(string country_code, string netflix_id);
    }
}
