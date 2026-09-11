using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore.Metadata.Builders;
using Microsoft.EntityFrameworkCore;

namespace Shipping.Identity
{
    public class ShippingUserRole : IdentityUserRole<string>
    {
        public virtual ShippingUser User { get; set; }
        public virtual ShippingRole Role { get; set; }
    }

    public sealed class ShippingUserRoleConfig : IEntityTypeConfiguration<ShippingUserRole>
    {
        public void Configure(EntityTypeBuilder<ShippingUserRole> builder)
        {
            builder.HasOne(userRole => userRole.Role)
                .WithMany(role => role.UserRoles)
                .HasForeignKey(userRole => userRole.RoleId)
                .IsRequired()
                .OnDelete(DeleteBehavior.Restrict);
        }
    }
}
