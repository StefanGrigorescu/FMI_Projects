using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Repositories.IRepositories
{
    public interface ITitleCountriesRepository: IGenericRepository<TitleCountry, TitleCountryModel>
    {
        Task<List<TitleCountryModel>> GetAllByNetflixIdAsync(string netflixId);

    }
}
