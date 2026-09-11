using Microsoft.AspNetCore.Identity;
using System.Collections.ObjectModel;

namespace Shipping.Identity
{
    public class ShippingRole : IdentityRole
    {
        public static readonly string Manager = "manager";
        public static readonly string ShippingDriver = "shipping_driver";

        public static readonly IReadOnlyCollection<string> AllRoles =
            new ReadOnlyCollection<string>(new[] { Manager, ShippingDriver });

        public virtual ICollection<ShippingUserRole> UserRoles { get; set; }
    }
}
