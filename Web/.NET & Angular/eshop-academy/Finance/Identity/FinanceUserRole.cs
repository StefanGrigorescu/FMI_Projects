using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Finance.Identity
{
    public class FinanceUserRole : IdentityUserRole<string>
    {
        public virtual FinanceUser User { get; set; }
        public virtual FinanceRole Role { get; set; }
    }

    public sealed class FinanceUserRoleConfig : IEntityTypeConfiguration<FinanceUserRole>
    {
        public void Configure(EntityTypeBuilder<FinanceUserRole> builder)
        {
            builder.HasOne(userRole => userRole.Role)
                .WithMany(role => role.UserRoles)
                .HasForeignKey(userRole => userRole.RoleId)
                .IsRequired()
                .OnDelete(DeleteBehavior.Restrict);
        }
    }
}
