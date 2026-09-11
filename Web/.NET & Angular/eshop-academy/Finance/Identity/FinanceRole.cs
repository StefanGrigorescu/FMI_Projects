using Finance.Identity;
using Microsoft.AspNetCore.Identity;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace Finance.Identity
{
    public class FinanceRole : IdentityRole
    {
        public static readonly string Manager = "manager";
        public static readonly string FinanceRepresentative = "finance_representative";

        public static readonly IReadOnlyCollection<string> AllRoles =
            new ReadOnlyCollection<string>(new[] { Manager, FinanceRepresentative });

        public virtual ICollection<FinanceUserRole> UserRoles { get; set; }
    }
}

