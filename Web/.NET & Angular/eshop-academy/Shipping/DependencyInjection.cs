using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Shipping.ShippingLabels.Commands;
using Shipping.Utils;

namespace Shipping
{
    public static class DependencyInjection
    {
        public static IServiceCollection AddShipping(this IServiceCollection services)
        {
            services.AddDatabaseServices();
            services.AddTransient<CreateShippingLabel>();
            services.AddMediator(options =>
            {
                options.ServiceLifetime = ServiceLifetime.Scoped;
            });
            services.AddSingleton<IDateTimeFactory, DateTimeFactory>();
            return services;
        }

        private static IServiceCollection AddDatabaseServices(this IServiceCollection services)
        {
            const string connectionString =
                "Server=(localdb)\\MSSQLLocalDB;initial catalog=EShopAcademy;trusted_connection=true;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=True;Connection Timeout=30";

            services.AddDbContext<ShippingDbContext>(options =>
                options
                    .UseSqlServer(
                        connectionString,
                        b => b.MigrationsAssembly(typeof(ShippingDbContext).Assembly.FullName))
            );

            return services;
        }
    }
}
