using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class cardGameContext: DbContext
    {
        //constructorul
        public cardGameContext(DbContextOptions<cardGameContext> optionsParam): base(optionsParam) { }

        public DbSet<Admin> Admins { get; set; }
        public DbSet<Player> Players { get; set; }
        public DbSet<Deck> Decks { get; set; }
        public DbSet<DeckCard> Deck_Cards { get; set; }
        public DbSet<Card> Cards { get; set; }
        public DbSet<CardAbility> Card_Abilities { get; set; }
        public DbSet<Ability> Abilities { get; set; }
        public DbSet<StrategicDetails> Strategic_Details { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Admin>().ToTable("Admins");
            modelBuilder.Entity<Player>().ToTable("Players");
            modelBuilder.Entity<Deck>().ToTable("Decks");
            modelBuilder.Entity<DeckCard>().ToTable("Deck_Cards");
            modelBuilder.Entity<Card>().ToTable("Cards");
            modelBuilder.Entity<CardAbility>().ToTable("Card_Abilities");
            modelBuilder.Entity<Ability>().ToTable("Abilities");
            modelBuilder.Entity<StrategicDetails>().ToTable("Strategic_Details");

            // explicit primary key for entity:
            modelBuilder.Entity<Admin>()
                .HasKey(admin => admin.admin_code);

            // composite keys for relational entities:
            modelBuilder.Entity<CardAbility>()
                .HasKey(cardAbility => new { cardAbility.ability_name, cardAbility.card_name });

            modelBuilder.Entity<DeckCard>()
                .HasKey(deckCard => new { deckCard.card_name, deckCard.deck_name });

            
            // Relations:
            modelBuilder.Entity<Admin>(entity => {
                entity.HasOne(admin => admin.player)
                .WithOne(player => player.admin)
                .HasForeignKey<Admin>(admin => admin.player_name);
            });

            modelBuilder.Entity<Deck>()
                .HasOne(deck => deck.owner)
                .WithMany(player => player.decks)
                .HasForeignKey(deck => deck.player_name);

            modelBuilder.Entity<DeckCard>()
                .HasOne(deckCard => deckCard.deck)
                .WithMany(deck => deck.deckCards)
                .HasForeignKey(deckCard => deckCard.deck_name);

            modelBuilder.Entity<StrategicDetails>()
                .HasOne(sd => sd.deckCard)
                .WithOne(deckCard => deckCard.strategicDetails)
                .HasForeignKey<StrategicDetails>(sd => new { sd.card_name, sd.deck_name });

            modelBuilder.Entity<DeckCard>()
                .HasOne(deckCard => deckCard.card)
                .WithMany(card => card.deckCards)
                .HasForeignKey(deckCard => deckCard.card_name);

            modelBuilder.Entity<CardAbility>()
                .HasOne(cardAbility => cardAbility.card)
                .WithMany(card => card.cardAbilities)
                .HasForeignKey(cardAbility => cardAbility.card_name);

            modelBuilder.Entity<CardAbility>()
                .HasOne(cardAbility => cardAbility.ability)
                .WithMany(ability => ability.cardAbilities)
                .HasForeignKey(cardAbility => cardAbility.ability_name);


            base.OnModelCreating(modelBuilder);
        }
    }
}
