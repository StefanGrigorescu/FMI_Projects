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
    public class FinanceUser : IdentityUser
    {
        public DateTime CreatedOn { get; set; } = DateTime.UtcNow;
        public virtual ICollection<FinanceUserRole> UserRoles { get; set; }
    }

    public sealed class FinanceUserConfig : IEntityTypeConfiguration<FinanceUser>
    {
        public void Configure(EntityTypeBuilder<FinanceUser> builder)
        {
            builder.HasMany(user => user.UserRoles)
                .WithOne(userRole => userRole.User)
                .HasForeignKey(userRole => userRole.UserId)
                .IsRequired()
                .OnDelete(DeleteBehavior.Cascade);

            builder.Property(user => user.UserName)
                .HasMaxLength(69)
                .IsRequired();

            builder.Property(user => user.Email)
                .HasMaxLength(320)
                .IsRequired();

            builder.Property(user => user.PhoneNumber)
                .HasMaxLength(69);

            builder.Property(user => user.CreatedOn)
                .HasDefaultValue(new DateTime(2023, 5, 16, 0, 0, 0, 0, DateTimeKind.Utc));
        }
    }
}

