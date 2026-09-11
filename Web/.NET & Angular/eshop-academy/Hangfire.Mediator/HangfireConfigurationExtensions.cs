using Newtonsoft.Json;

namespace Hangfire.Mediator
{
    public static class HangfireConfigurationExtensions
    {
        public static void UseMediator(this IGlobalConfiguration config)
        {
            JsonSerializerSettings jsonSettings = new()
            { 
                TypeNameHandling = TypeNameHandling.All,
            };
            config.UseSerializerSettings(jsonSettings);
        }
    }
}
