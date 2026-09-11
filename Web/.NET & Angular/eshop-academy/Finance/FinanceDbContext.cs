using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.AspNetCore.Identity;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Finance.Identity;
using Microsoft.EntityFrameworkCore;
using Finance.Utils;
using Microsoft.EntityFrameworkCore.Metadata;

namespace Finance
{
    public sealed class FinanceDbContext : IdentityDbContext
        <FinanceUser, FinanceRole, string, IdentityUserClaim<string>,
        FinanceUserRole, IdentityUserLogin<string>, IdentityRoleClaim<string>, IdentityUserToken<string>>
    {
        private IDateTimeFactory _dateTimeFactory;

        public FinanceDbContext(
            DbContextOptions<FinanceDbContext> options,
            IDateTimeFactory dateTimeFactory) :
            base(options)
        {
            _dateTimeFactory = dateTimeFactory;
        }

        protected override void OnModelCreating(ModelBuilder builder)
        {
            base.OnModelCreating(builder);

            builder.HasDefaultSchema("Sales");

            builder.ApplyConfiguration(new FinanceUserConfig());
            builder.ApplyConfiguration(new FinanceUserRoleConfig());


            ConfigureCreatedOnPropertyForAllEntities(builder);
            ConfigureLastUpdatedOnPropertyForAllEntities(builder);
        }

        public override int SaveChanges()
        {
            SetCreatedOnPropertyForNewEntities();
            SetLastUpdatedOnPropertyForUpdatedEntities();
            return base.SaveChanges();
        }
        public override async Task<int> SaveChangesAsync(CancellationToken cancellationToken = default)
        {
            SetCreatedOnPropertyForNewEntities();
            SetLastUpdatedOnPropertyForUpdatedEntities();
            return await base.SaveChangesAsync(cancellationToken);
        }


        private void ConfigureCreatedOnPropertyForAllEntities(ModelBuilder builder)
        {
            IEnumerable<IMutableEntityType> entityTypes = builder
                .Model
                .GetEntityTypes()
                .Where(entityType => typeof(EntityBase).IsAssignableFrom(entityType.ClrType));

            foreach (IMutableEntityType entityType in entityTypes)
            {
                builder.Entity(entityType.ClrType)
                    .Property(nameof(EntityBase.CreatedOn))
                    .IsRequired()
                    .HasDefaultValue(new DateTime(1, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc));
            }
        }

        private void SetCreatedOnPropertyForNewEntities()
        {
            IEnumerable<EntityBase?> addedEntities = ChangeTracker
                .Entries()
                .Where(entry =>
                    entry.State == EntityState.Added &&
                    entry.Entity is EntityBase)
                .Select(entry => entry.Entity as EntityBase);

            foreach (EntityBase? entity in addedEntities)
            {
                /*
                 * No entity should be null in general 
                 * since we first filtered with "is" keyword 
                 * then casted with "as" keyword
                 */
                if (entity is null)
                {
                    continue;
                }

                entity.CreatedOn = _dateTimeFactory.UtcNow();
            }
        }

        private void ConfigureLastUpdatedOnPropertyForAllEntities(ModelBuilder builder)
        {
            IEnumerable<IMutableEntityType> entityTypes = builder
                .Model
                .GetEntityTypes()
                .Where(entityType => typeof(EntityBase).IsAssignableFrom(entityType.ClrType));

            foreach (IMutableEntityType entityType in entityTypes)
            {
                builder.Entity(entityType.ClrType)
                    .Property(nameof(EntityBase.LastUpdatedOn))
                    .HasDefaultValue(null);
            }
        }

        private void SetLastUpdatedOnPropertyForUpdatedEntities()
        {
            IEnumerable<EntityBase?> addedEntities = ChangeTracker
                .Entries()
                .Where(entry =>
                    entry.State == EntityState.Modified &&
                    entry.Entity is EntityBase)
                .Select(entry => entry.Entity as EntityBase);

            foreach (EntityBase? entity in addedEntities)
            {
                /*
                 * No entity should be null in general 
                 * since we first filtered with "is" keyword 
                 * then casted with "as" keyword
                 */
                if (entity is null)
                {
                    continue;
                }

                entity.LastUpdatedOn = _dateTimeFactory.UtcNow();
            }
        }
    }
}
