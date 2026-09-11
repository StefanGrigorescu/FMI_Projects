using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Microsoft.IdentityModel.Tokens;
using Microsoft.OpenApi.Models;
using Movie4U.EntitiesModels.Entities;
using Movie4U.Managers;
using Movie4U.Managers.IManagers;
using Movie4U.Repositories;
using Movie4U.Repositories.IRepositories;
using System.Collections.Generic;
using System.Text;

namespace Movie4U
{
    public class Startup
    {
        public IConfiguration configuration { get; }

        /**<summary>
         * Constructor.
         * </summary>*/
        public Startup(IConfiguration configuration)
        {
            this.configuration = configuration;
        }

        /**<summary>
         * This method gets called by the runtime. Use this method to add services to the container.
         * </summary>*/ 
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllers();
            services.AddSwaggerGen(c =>
            {
                c.SwaggerDoc("v1", new OpenApiInfo { Title = "Movie4U", Version = "v1" });
                c.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme
                {
                    Description = @"JWT Authorization header using the Bearer scheme. \r\n\r\n
                        Enter 'Bearer' [space] and then your token in the text input below.
                        \r\n\r\nExample: 'Bearer 12345abcdef'",
                    Name = "Authorization",
                    In = ParameterLocation.Header,
                    Type = SecuritySchemeType.ApiKey,
                    Scheme = "Bearer"
                });

                c.AddSecurityRequirement(new OpenApiSecurityRequirement
                {
                    {
                        new OpenApiSecurityScheme
                        {
                            Reference = new OpenApiReference
                            {
                                Type = ReferenceType.SecurityScheme,
                                Id = "Bearer"
                            },
                            Scheme = "oauth2",
                            Name = "Bearer",
                            In = ParameterLocation.Header
                        },
                        new List<string>()
                    }
                });
            });

            services.AddDbContext<Movie4UContext>(options => options
            .UseLoggerFactory(LoggerFactory.Create(builder => builder.AddConsole()))  
            .UseSqlServer(configuration["ConnectionStrings:DefaultConnection"]));


            // since we have added IdentityDbContext, we add this to specify we use the user and role defined by us
            services.AddIdentity<User, Role>()
                .AddEntityFrameworkStores<Movie4UContext>();

            services.AddAuthentication(options =>
            {
                options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
                options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
            })
                .AddJwtBearer("AuthScheme", options =>
                {
                    options.SaveToken = true;
                    var secret =
                        configuration
                        .GetSection("Jwt")
                        .GetSection("SecretKey")
                        .Get<string>();
                    options.TokenValidationParameters = new TokenValidationParameters
                    {
                        ValidateIssuerSigningKey = true,
                        ValidateLifetime = true,
                        RequireExpirationTime = true,
                        IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(secret)),
                        ValidateIssuer = false,
                        ValidateAudience = false
                    };
                });

            services.AddAuthorization(options =>
            {
                // add BasicUserPolicy for BasicUser role
                options
                    .AddPolicy("BasicUserPolicy", policy =>
                        policy
                        .RequireRole("BasicUser", "Admin")
                        .RequireAuthenticatedUser()
                        .AddAuthenticationSchemes("AuthScheme")
                        .Build());
            });

            services.AddAuthorization(options =>
            {
                // add AdminPolicy for Admin role
                options
                    .AddPolicy("AdminPolicy", policy =>
                        policy
                        .RequireRole("Admin")
                        .RequireAuthenticatedUser()
                        .AddAuthenticationSchemes("AuthScheme")
                        .Build());
            });


            services.AddControllersWithViews()
                .AddNewtonsoftJson(optionsParam =>
                optionsParam.SerializerSettings.ReferenceLoopHandling = Newtonsoft.Json.ReferenceLoopHandling.Ignore);

            services.AddCors(options =>
            {
                options.AddPolicy(name: "_allowSpecificOrigins",
                    builder =>
                    {
                        builder
                        .WithOrigins("localhost:4200",
                        "http://localhost:4200",
                        "https://localhost:4200",
                        "localhost:4200/",
                        "http://localhost:4200/",
                        "https://localhost:4200/")
                        .AllowAnyMethod()
                        .AllowAnyHeader();
                    });
            });

            services.AddScoped<ITokensManager, TokensManager>();

            services.AddScoped<IAuthenticationManager, AuthenticationManager>();

            services.AddScoped<IWatchersRepository, WatchersRepository>();
            services.AddScoped<IWatchersManager, WatchersManager>();

            services.AddScoped<IGenresRepository, GenresRepository>();
            services.AddScoped<IGenresManager, GenresManager>();

            services.AddScoped<ICountriesRepository, CountriesRepository>();
            services.AddScoped<ICountriesManager, CountriesManager>();

            services.AddScoped<ITitleImagesRepository, TitleImagesRepository>();
            services.AddScoped<ITitleImagesManager, TitleImagesManager>();

            services.AddScoped<ITitleGenresRepository, TitleGenresRepository>();
            services.AddScoped<ITitleGenresManager, TitleGenresManager>();

            services.AddScoped<ITitleCountriesRepository, TitleCountriesRepository>();
            services.AddScoped<ITitleCountriesManager, TitleCountriesManager>();

            services.AddScoped<ITitlesRepository, TitlesRepository>();
            services.AddScoped<ITitlesManager, TitlesManager>();

            services.AddScoped<IWatcherTitlesRepository, WatcherTitlesRepository>();
            services.AddScoped<IWatcherTitlesManager, WatcherTitlesManager>();

            services.AddScoped<IWatcherGenresRepository, WatcherGenresRepository>();
            services.AddScoped<IWatcherGenresManager, WatcherGenresManager>();
        }

        /**<summary>
         * This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
         * </summary>*/
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                    app.UseDeveloperExceptionPage();
                    app.UseSwagger();
                    app.UseSwaggerUI(c => c.SwaggerEndpoint("/swagger/v1/swagger.json", "Movie4U v1"));
                }
            else
            {
                app.UseExceptionHandler("/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }

            app.UseCors("_allowSpecificOrigins");

            app.UseHttpsRedirection();        // http requests redirected to https
            /*app.UseStaticFiles();*/

            app.UseRouting();

            app.UseAuthorization();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllers();
            });
        }
    }
}
