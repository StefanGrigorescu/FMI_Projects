using Microsoft.EntityFrameworkCore;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Movie4U.EntitiesModels.Entities;
using Movie4U.Repositories.IRepositories;
using Movie4U.Utilities;
using Movie4U.EntitiesModels;

namespace Movie4U.Repositories
{
    public class GenericRepository<EntityType, ModelType> : IGenericRepository<EntityType, ModelType>
        where EntityType : EntitiesModelsBase<EntityType, ModelType>
        where ModelType : EntitiesModelsBase<EntityType, ModelType>, new()
    {
        /**<summary>
         * The context.
         * </summary>*/
        internal Movie4UContext db;
        internal DbSet<EntityType> entities;

        /**<summary>
         * Constructor.
         * </summary>*/
        public GenericRepository(Movie4UContext db)
        {
            this.db = db;
            entities = db.Set<EntityType>();
        }

        public virtual IQueryable<EntityType> GetAllDbQueryable()
        {
            return entities;
        }

        public virtual List<ModelType> GetAll()
        {
            var entitiesList = entities.AsNoTracking().ToList();

            return CastUtility.ToModelsList<EntityType, ModelType>(entitiesList);
        }

        public virtual async Task<List<ModelType>> GetAllAsync()
        {
            var entitiesList = await entities.AsNoTracking().ToListAsync();

            return CastUtility.ToModelsList<EntityType, ModelType>(entitiesList);
        }

        public virtual IEnumerable<EntityType> GetAllDb()
        {
            return entities;
        }

        public async Task<List<EntityType>> GetAllDbAsync()
        {
            return await entities.ToListAsync();
        }

        public ModelType GetOneById(object id)
        {
            var entity = entities.Find(id);

            return CastUtility.ToModel<EntityType, ModelType>(entity);
        }

        public virtual async Task<ModelType> GetOneByIdAsync(object id)
        {
            var entity = await entities.FindAsync(id);

            return CastUtility.ToModel<EntityType, ModelType>(entity);
        }

        public ModelType GetOneById(object id1, object id2)
        {
            var entity = entities.Find(id1, id2);

            return CastUtility.ToModel<EntityType, ModelType>(entity);
        }

        public virtual async Task<ModelType> GetOneByIdAsync(object id1, object id2)
        {
            var entity = await entities.FindAsync(id1, id2);

            return CastUtility.ToModel<EntityType, ModelType>(entity);
        }

        public virtual EntityType GetOneDbById(object id)
        {
            return entities.Find(id);
        }

        public async Task<EntityType> GetOneDbByIdAsync(object id)
        {
            return await entities.FindAsync(id);
        }

        public virtual EntityType GetOneDbById(object id1, object id2)
        {
            return entities.Find(id1, id2);
        }

        public async Task<EntityType> GetOneDbByIdAsync(object id1, object id2)
        {
            return await entities.FindAsync(id1, id2);
        }

        public virtual void Insert(EntityType entity)
        {
            entities.Add(entity);
        }

        public virtual async Task<EntityType> InsertAsync(EntityType entity)
        {
            await entities.AddAsync(entity);
            await db.SaveChangesAsync();
            return entity;
        }

        public virtual async Task UpdateAsync(EntityType entityToUpdate)
        {
            entities.Attach(entityToUpdate);
            db.Entry(entityToUpdate).State = EntityState.Modified;
            await db.SaveChangesAsync();
        }

        public virtual async Task DeleteAsync(object id)
        {
            var entityToDelete = await entities.FindAsync(id);
            await DeleteAsync(entityToDelete);
            await db.SaveChangesAsync();
        }

        public virtual async Task DeleteAsync(EntityType entityToDelete)
        {
            if (db.Entry(entityToDelete).State == EntityState.Detached)
                entities.Attach(entityToDelete);
            entities.Remove(entityToDelete);
            await db.SaveChangesAsync();
        }

    }
}
