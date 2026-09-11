using Movie4U.EntitiesModels.Entities;
using Movie4U.EntitiesModels.Models;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories.IRepositories;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers
{
    public class TitlesManager: ITitlesManager
    {
        private readonly ITitlesRepository repo;
        private readonly ITitleCountriesManager titleCountriesManager;
        private readonly ITitleGenresManager titleGenresManager;
        private readonly ITitleImagesManager titleImagesManager; 

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitlesManager(ITitlesRepository repo, ITitleCountriesManager countriesManager, ITitleGenresManager genresManager, ITitleImagesManager imagesManager)
        {
            this.repo = repo;
            this.titleCountriesManager = countriesManager;
            this.titleGenresManager = genresManager;
            this.titleImagesManager = imagesManager;
        }


        private async Task<bool> FillModelsLists(TitleModel titleModel)
        {
            var netflixId = titleModel.netflix_id;
            titleModel.countryModels = await titleCountriesManager.GetAllCountriesByNetflixIdAsync(netflixId);
            titleModel.genreModels = await titleGenresManager.GetAllGenresByNetflixIdAsync(netflixId);
            titleModel.titleImageModels = await titleImagesManager.GetAllByNetflixIdAsync(netflixId);

            return true;
        }

        public async Task<List<TitleModel>> GetAllAsync()
        {
            var titleModels = await repo.GetAllAsync();

            foreach (var titleModel in titleModels)
                await FillModelsLists(titleModel);

            return titleModels;
        }

        public async Task<TitleModel> GetOneByIdAsync(string netflix_id)
        {
            var titleModel = await repo.GetOneByIdAsync(netflix_id);
            if(titleModel != null)
                await FillModelsLists(titleModel);

            return titleModel;
        }

        public async Task Create(TitleModelParameter titleModelParam)
        {
            Title newTitle = new Title(titleModelParam);

            await repo.InsertAsync(newTitle);
        }

        public async Task Update(TitleModelParameter titleModelParam)
        {
            Title updateTitle = await repo.GetOneDbByIdAsync(titleModelParam.netflix_id);
            updateTitle.Copy(titleModelParam);

            await repo.UpdateAsync(updateTitle);
        }

        public async Task Delete(string netflix_id)
        {
            Title delTitle = await repo.GetOneDbByIdAsync(netflix_id);

            if (delTitle != null)
                await repo.DeleteAsync(delTitle);
        }

    }
}
