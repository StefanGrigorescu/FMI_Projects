using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Repositories.IRepositories;
using Movie4U.Utilities;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Movie4U.Repositories
{
    public class TitleImagesRepository: GenericRepository<TitleImage, TitleImageModel>, ITitleImagesRepository
    {
        /**
         * <summary>
         * Constructor.
         * </summary>>
         */
        public TitleImagesRepository(Movie4UContext db) : base(db) { }


        public async Task<List<TitleImageModel>> GetAllByNetflixIdAsync(string netflixId)
        {
            var allDb = await GetAllDbAsync();
            var titleImages = 
                allDb
                .Where(ti => ti.netflix_id == netflixId)
                .ToList();

            return CastUtility.ToModelsList<TitleImage, TitleImageModel>(titleImages);
        }

    }
}
