using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Sales.Identity;
using Sales.Orders;
using Sales.Utils;
using Microsoft.EntityFrameworkCore.Metadata;

namespace Sales
{
    public sealed class SalesDbContext : IdentityDbContext
        <SalesUser, SalesRole, string, IdentityUserClaim<string>,
        SalesUserRole, IdentityUserLogin<string>, IdentityRoleClaim<string>, IdentityUserToken<string>>
    {
        private IDateTimeFactory _dateTimeFactory;

        public SalesDbContext(
            DbContextOptions<SalesDbContext> options,
            IDateTimeFactory dateTimeFactory) :
            base(options)
        {
            _dateTimeFactory = dateTimeFactory;
        }

        //public DbSet<Order> Orders { get; set; }


        protected override void OnModelCreating(ModelBuilder builder)
        {
            base.OnModelCreating(builder);

            builder.HasDefaultSchema("Sales");

            builder.ApplyConfiguration(new SalesUserConfig());
            builder.ApplyConfiguration(new SalesUserRoleConfig());


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
