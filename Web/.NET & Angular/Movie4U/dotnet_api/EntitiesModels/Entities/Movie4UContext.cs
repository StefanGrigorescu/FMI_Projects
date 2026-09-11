using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;
using Movie4U.ExtensionMethods;
using System;

namespace Movie4U.EntitiesModels.Entities
{
    public class Movie4UContext : IdentityDbContext
        <User, Role, string, IdentityUserClaim<string>,
        UserRole, IdentityUserLogin<string>, IdentityRoleClaim<string>, IdentityUserToken<string>>
    {
        /**<summary>
         * Constructor.
         * </summary>*/
        public Movie4UContext(DbContextOptions<Movie4UContext> options) : base(options) { }

        // entities
        public DbSet<Watcher> Watchers { get; set; }
        public DbSet<Country> Countries { get; set; }
        public DbSet<Genre> Genres { get; set; }
        public DbSet<Title> Titles { get; set; }
        public DbSet<TitleCountry> TitleCountries { get; set; }
        public DbSet<TitleGenre> TitleGenres { get; set; }
        public DbSet<TitleImage> TitleImages { get; set; }
        public DbSet<WatcherTitle> WatcherTitles { get; set; }
        public DbSet<WatcherGenre> WatcherGenres { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            if (modelBuilder == null)
                throw new ArgumentException("modelBuilder argument is null");

            base.OnModelCreating(modelBuilder);

            // entities:
            modelBuilder.Entity<Watcher>().ToTable("Watchers");
            modelBuilder.Entity<Country>().ToTable("Countries");
            modelBuilder.Entity<Genre>().ToTable("Genres");
            modelBuilder.Entity<Title>().ToTable("Title_Details");
            modelBuilder.Entity<TitleCountry>().ToTable("Title_Countries");
            modelBuilder.Entity<TitleGenre>().ToTable("Title_Genres");
            modelBuilder.Entity<TitleImage>().ToTable("Title_Images");
            modelBuilder.Entity<WatcherTitle>().ToTable("Watcher_Titles");
            modelBuilder.Entity<WatcherGenre>().ToTable("Watcher_Genres");

            // composite keys for relational entities:
            modelBuilder.Entity<TitleCountry>().HasKey(tc => new { tc.netflix_id, tc.country_code });
            modelBuilder.Entity<TitleGenre>().HasKey(tg => new { tg.netflix_id, tg.genre_id });
            modelBuilder.Entity<WatcherTitle>().HasKey(wt => new { wt.watcher_name, wt.netflix_id });
            modelBuilder.Entity<WatcherGenre>().HasKey(wg => new { wg.watcher_name, wg.genre_id });

            // relationships:
            modelBuilder.Entity<Watcher>()
                .HasOne(watcher => watcher.user)
                .WithOne(user => user.watcher)
                .HasForeignKey<Watcher>(watcher => watcher.userId);

            modelBuilder.Entity<WatcherTitle>()
                .HasOne(wt => wt.watcher)
                .WithMany(watcher => watcher.watcherTitles)
                .HasForeignKey(wt => wt.watcher_name);

            modelBuilder.Entity<WatcherTitle>()
                .HasOne(wt => wt.title)
                .WithMany(title => title.watcherTitles)
                .HasForeignKey(wt => wt.netflix_id);

            modelBuilder.Entity<WatcherGenre>()
                .HasOne(wg => wg.watcher)
                .WithMany(watcher => watcher.watcherGenres)
                .HasForeignKey(wg => wg.watcher_name);

            modelBuilder.Entity<WatcherGenre>()
                .HasOne(wg => wg.genre)
                .WithMany(genre => genre.watcherGenres)
                .HasForeignKey(wg => wg.genre_id);

            modelBuilder.Entity<TitleCountry>()
                .HasOne(tc => tc.Country)
                .WithMany(country => country.titleCountries)
                .HasForeignKey(tc => tc.country_code);

            modelBuilder.Entity<TitleCountry>()
                .HasOne(tc => tc.title)
                .WithMany(title => title.titleCountries)
                .HasForeignKey(tc => tc.netflix_id);

            modelBuilder.Entity<TitleGenre>()
                .HasOne(tg => tg.Genre)
                .WithMany(Genre => Genre.titleGenres)
                .HasForeignKey(tg => tg.genre_id);

            modelBuilder.Entity<TitleGenre>()
                .HasOne(tg => tg.title)
                .WithMany(title => title.titleGenres)
                .HasForeignKey(tg => tg.netflix_id);

            modelBuilder.Entity<TitleImage>()
                .HasOne(ti => ti.title)
                .WithMany(title => title.titleImages)
                .HasForeignKey(ti => ti.netflix_id);


            // conventions (from ExtensionMethods.ContextExtension):
            modelBuilder.DeleteBehaviorConvention(DeleteBehavior.Cascade);

        }
    }

}