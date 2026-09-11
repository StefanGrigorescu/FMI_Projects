using Hangfire;
using Hangfire.Mediator;
using Sales;
using Shipping;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.

builder.Services.AddControllers();
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

builder.Services.AddHangfireMediator();
builder.Services.AddSales();
builder.Services.AddShipping();

const string hangfireConnectionString =
    "Server=(localdb)\\MSSQLLocalDB;initial catalog=EShopAcademy_Hangfire;trusted_connection=true;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=True;Connection Timeout=30";
builder.Services.AddHangfire(configuration =>
{
    configuration.UseSqlServerStorage(hangfireConnectionString);
    configuration.UseMediator();
});

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();

app.UseRouting();

app.UseAuthentication();
app.UseAuthorization();

app.MapControllers();

app.Run();
