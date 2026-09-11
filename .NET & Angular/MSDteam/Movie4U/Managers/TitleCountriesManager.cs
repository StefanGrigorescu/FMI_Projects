using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories.IRepositories;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class TitleCountriesManager: ITitleCountriesManager
    {

        private readonly ITitleCountriesRepository repo;
        private readonly ICountriesManager countriesManager;

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleCountriesManager(ITitleCountriesRepository repo, ICountriesManager countriesManager)
        {
            this.repo = repo;
            this.countriesManager = countriesManager;
        }


        public async Task<List<TitleCountryModel>> GetAllAsync()
        {
            return await repo.GetAllAsync();
        }

        public async Task<List<TitleCountryModel>> GetAllByNetflixIdAsync(string netflixId)
        {
            return await repo.GetAllByNetflixIdAsync(netflixId);
        }

        public async Task<List<CountryModel>> GetAllCountriesByNetflixIdAsync(string netflixId)
        {
            var titleCountries = await GetAllByNetflixIdAsync(netflixId);

            var countries = new List<CountryModel>();
            foreach (var titleCountry in titleCountries)
            {
                var country = await countriesManager.GetOneByIdAsync(titleCountry.country_code);
                countries.Add(country);
            }

            return countries;
        }

        public async Task<TitleCountryModel> GetOneByIdAsync(string country_code, string netflix_id)
        {
            return await repo.GetOneByIdAsync(country_code, netflix_id);
        }

        public async Task Create(TitleCountryModel titleCountryModel)
        {
            TitleCountry newTitleGenre = new TitleCountry(titleCountryModel);

            await repo.InsertAsync(newTitleGenre);
        }

        public async Task Update(TitleCountryModel titleCountryModel)
        {
            TitleCountry updateTitleCountry = await repo.GetOneDbByIdAsync(titleCountryModel.country_code, titleCountryModel.netflix_id);
            updateTitleCountry.Copy(titleCountryModel);

            await repo.UpdateAsync(updateTitleCountry);
        }

        public async Task Delete(string country_code, string netflix_id)
        {
            TitleCountry delTitleCountry = await repo.GetOneDbByIdAsync(country_code, netflix_id);

            if (delTitleCountry != null)
                await repo.DeleteAsync(delTitleCountry);
        }

    }
}
