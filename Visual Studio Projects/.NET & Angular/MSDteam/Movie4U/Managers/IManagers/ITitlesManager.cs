using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Movie4U.Managers.IManagers
{
    public interface ITitlesManager
    {
        Task<List<TitleModel>> GetAllAsync();

        Task<TitleModel> GetOneByIdAsync(string netflix_id);

        Task Create(TitleModelParameter titleModelParam);

        Task Update(TitleModelParameter titleModelParam);

        Task Delete(string netflix_id);
    }
}
