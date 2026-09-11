using Hangfire;
using Hangfire.Mediator;
using Sales;
using Shipping;

IHostBuilder builder = Host.CreateDefaultBuilder(args);

builder.ConfigureServices(services =>
{
    services.AddSales();
    services.AddShipping();

    const string hangfireConnectionString =
        "Server=(localdb)\\MSSQLLocalDB;initial catalog=EShopAcademy_Hangfire;trusted_connection=true;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=True;Connection Timeout=30";
    services.AddHangfire(configuration =>
    {
        configuration.UseSqlServerStorage(hangfireConnectionString);
        configuration.UseMediator();
    });

    services.AddHangfireServer();
});
