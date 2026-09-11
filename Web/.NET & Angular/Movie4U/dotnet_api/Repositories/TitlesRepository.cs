using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Repositories.IRepositories;

namespace Movie4U.Repositories
{
    public class TitlesRepository : GenericRepository<Title, TitleModel>, ITitlesRepository
    {
        /**<summary>
         * Constructor.
         * </summary>*/
        public TitlesRepository(Movie4UContext db) : base(db) { }

    }
}
