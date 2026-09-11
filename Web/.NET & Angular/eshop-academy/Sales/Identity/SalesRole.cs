using Microsoft.AspNetCore.Identity;
using System.Collections.ObjectModel;

namespace Sales.Identity
{
    public class SalesRole : IdentityRole
    {
        public static readonly string Manager = "manager";
        public static readonly string SalesRepresentative = "sales_representative";

        public static readonly IReadOnlyCollection<string> AllRoles =
            new ReadOnlyCollection<string>(new[] { Manager, SalesRepresentative });

        public virtual ICollection<SalesUserRole> UserRoles { get; set; }
    }
}
