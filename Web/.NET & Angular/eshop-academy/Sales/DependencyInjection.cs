using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Sales.Utils;

namespace Sales
{
    public static class DependencyInjection
    {
        public static IServiceCollection AddSales(this IServiceCollection services)
        {
            services.AddDatabaseServices();
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

            services.AddDbContext<SalesDbContext>(options =>
                options
                    .UseSqlServer(
                        connectionString,
                        b => b.MigrationsAssembly(typeof(SalesDbContext).Assembly.FullName))
            );

            return services;
        }
    }
}
