using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using Microsoft.EntityFrameworkCore;

namespace Sales.Identity
{
    public class SalesUserRole : IdentityUserRole<string>
    {
        public virtual SalesUser User { get; set; }
        public virtual SalesRole Role { get; set; }
    }

    public sealed class SalesUserRoleConfig : IEntityTypeConfiguration<SalesUserRole>
    {
        public void Configure(EntityTypeBuilder<SalesUserRole> builder)
        {
            builder.HasOne(userRole => userRole.Role)
                .WithMany(role => role.UserRoles)
                .HasForeignKey(userRole => userRole.RoleId)
                .IsRequired()
                .OnDelete(DeleteBehavior.Restrict);
        }
    }
}
