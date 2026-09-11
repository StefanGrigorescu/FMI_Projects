using Microsoft.Extensions.DependencyInjection;

namespace Hangfire.Mediator
{
    public static class DependencyInjection
    {
        public static IServiceCollection AddHangfireMediator(this IServiceCollection services)
        {
            services.AddMediator(options =>
            {
                options.ServiceLifetime = ServiceLifetime.Scoped;
            });

            return services;
        }
    }
}
