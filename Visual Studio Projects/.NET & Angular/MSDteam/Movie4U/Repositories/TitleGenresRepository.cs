using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Repositories.IRepositories;
using Movie4U.Utilities;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Movie4U.Repositories
{
    public class TitleGenresRepository: GenericRepository<TitleGenre, TitleGenreModel>, ITitleGenresRepository
    {
        /**<summary>
        * Constructor.
        * </summary>>*/
        public TitleGenresRepository(Movie4UContext db) : base(db) { }


        public async Task<List<TitleGenreModel>> GetAllByNetflixIdAsync(string netflixId)
        {
            var allDb = await GetAllDbAsync();
            var titleGenres = 
                allDb
                .Where(tg => tg.netflix_id == netflixId)
                .ToList();

            return CastUtility.ToModelsList<TitleGenre, TitleGenreModel>(titleGenres);
        }

    }
}
