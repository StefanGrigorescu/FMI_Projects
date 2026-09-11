using Microsoft.AspNetCore.Identity;

namespace Movie4U.EntitiesModels.Entities
{
    public class User : IdentityUser
    {
        public static object Identity { get; internal set; }
        public virtual Watcher watcher { get; set; }
    }
}
