using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Repositories.IRepositories;

namespace Movie4U.Repositories
{
    public class CountriesRepository : GenericRepository<Country, CountryModel>, ICountriesRepository
    {
        /**<summary>
         * Constructor.
         * </summary>*/
        public CountriesRepository(Movie4UContext db) : base(db) { }

    }
}
