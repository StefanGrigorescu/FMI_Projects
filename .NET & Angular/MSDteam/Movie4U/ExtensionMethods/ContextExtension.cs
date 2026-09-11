using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata;
using System;
using System.Linq;

namespace Movie4U.ExtensionMethods
{
    public static class ContextExtension
    {
        /**<summary>
         * Calls EntityLoop for the current modelBuilder and the lambda function passed as the action.
         * </summary>
         */
        public static void DeleteBehaviorConvention(this ModelBuilder modelBuilder, DeleteBehavior deleteBehavior)
        {
            modelBuilder
                .EntityLoop(entityType => entityType.GetForeignKeys()
                .Where(fk => fk.DeleteBehavior != deleteBehavior)
                .ToList()
                .ForEach(fk => fk.DeleteBehavior = deleteBehavior));
        }

        /** <summary>
         * Loops over entities of modelBuilder and applies action.
         * IMutableEntityType is like EntityType, but you can also modify the entityTypes.
         * </summary>*/
        public static void EntityLoop(this ModelBuilder modelBuilder, Action<IMutableEntityType> action)
        {
            foreach (var entityType in modelBuilder.Model.GetEntityTypes())
                action(entityType);
        }

    }
}
