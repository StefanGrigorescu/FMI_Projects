using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Repositories.IRepositories;

namespace Movie4U.Repositories
{
    public class GenresRepository : GenericRepository<Genre, GenreModel>, IGenresRepository
    {
        /**<summary>
         * Constructor.
         * </summary>*/
        public GenresRepository(Movie4UContext db) : base(db) { }

    }
}
