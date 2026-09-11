using Movie4U.EntitiesModels;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Movie4U.Repositories.IRepositories
{
    public interface IGenericRepository<TEntity, TModel> 
        where TEntity : EntitiesModelsBase<TEntity, TModel>
        where TModel : EntitiesModelsBase<TEntity, TModel>, new()
    {
        IQueryable<TEntity> GetAllDbQueryable();

        List<TModel> GetAll();
        Task<List<TModel>> GetAllAsync();

        IEnumerable<TEntity> GetAllDb();
        Task<List<TEntity>> GetAllDbAsync();

        TModel GetOneById(object id);
        Task<TModel> GetOneByIdAsync(object id);
        TModel GetOneById(object id1, object id2);
        Task<TModel> GetOneByIdAsync(object id1, object id2);

        TEntity GetOneDbById(object id);
        Task<TEntity> GetOneDbByIdAsync(object id);
        TEntity GetOneDbById(object id1, object id2);
        Task<TEntity> GetOneDbByIdAsync(object id1, object id2);

        void Insert(TEntity entity);
        Task<TEntity> InsertAsync(TEntity entity);

        Task UpdateAsync(TEntity entity);

        Task DeleteAsync(TEntity entity);
        Task DeleteAsync(object id);

    }
}
