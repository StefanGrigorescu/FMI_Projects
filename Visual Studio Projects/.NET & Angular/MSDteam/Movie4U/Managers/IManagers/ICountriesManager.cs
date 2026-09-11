using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface ICountriesManager
    {
        Task<List<CountryModel>> GetAllAsync();

        Task<CountryModel> GetOneByIdAsync(string country_code);

        Task Create(CountryModel countryModel);

        Task Update(CountryModel countryModel);

        Task Delete(string country_code);
    }
}
