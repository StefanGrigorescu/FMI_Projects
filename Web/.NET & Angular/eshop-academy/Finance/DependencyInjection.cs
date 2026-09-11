using Finance.Utils;
using Microsoft.Extensions.DependencyInjection;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Finance
{
    public static class DependencyInjection
    {
        //public static IServiceCollection AddFinance(this IServiceCollection services)
        //{
        //    services.AddDatabaseServices();
        //    services.AddMediator(options =>
        //    {
        //        options.ServiceLifetime = ServiceLifetime.Scoped;
        //    });
        //    services.AddSingleton<IDateTimeFactory, DateTimeFactory>();
        //    return services;
        //}

        //private static IServiceCollection AddDatabaseServices(this IServiceCollection services)
        //{
        //    const string connectionString =
        //        "Server=(localdb)\\MSSQLLocalDB;initial catalog=EShopAcademy;trusted_connection=true;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=True;Connection Timeout=30";

        //    services.AddDbContext<FinanceDbContext>(options =>
        //        options
        //            .UseSqlServer(
        //                connectionString,
        //                b => b.MigrationsAssembly(typeof(SalesDbContext).Assembly.FullName))
        //    );

        //    return services;
        //}
    }
}
