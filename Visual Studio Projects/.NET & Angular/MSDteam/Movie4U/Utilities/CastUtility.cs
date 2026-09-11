using Movie4U.EntitiesModels;
using System.Collections.Generic;

namespace Movie4U.Utilities
{
    public static class CastUtility
    {
        public static ModelType ToModel<EntityType, ModelType>(EntityType entity)
            where EntityType: EntitiesModelsBase<EntityType, ModelType>
            where ModelType: EntitiesModelsBase<EntityType, ModelType>, new()
        {
            var model = new ModelType();
            model.Copy(entity);

            return model;
        }

        public static List<ModelType> ToModelsList<EntityType, ModelType> (List<EntityType> entities) 
            where EntityType: EntitiesModelsBase<EntityType, ModelType>
            where ModelType: EntitiesModelsBase<EntityType, ModelType>, new()
        { 
            var models = new List<ModelType>();
            foreach (var entity in entities)
            {
                var model = new ModelType();
                model.Copy(entity);

                models.Add(model);
            }

            return models;
        }

    }
}
