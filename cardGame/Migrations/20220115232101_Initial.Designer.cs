﻿// <auto-generated />
using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Migrations;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;
using cardGame.Classes;

namespace cardGame.Migrations
{
    [DbContext(typeof(cardGameContext))]
    [Migration("20220115232101_Initial")]
    partial class Initial
    {
        protected override void BuildTargetModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("Relational:MaxIdentifierLength", 128)
                .HasAnnotation("ProductVersion", "5.0.13")
                .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn);

            modelBuilder.Entity("cardGame.Classes.Ability", b =>
                {
                    b.Property<string>("ability_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<string>("effect_details")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.Property<int>("trigger_condition")
                        .HasColumnType("int");

                    b.Property<int>("trigger_moment")
                        .HasColumnType("int");

                    b.HasKey("ability_name");

                    b.ToTable("Abilities");
                });

            modelBuilder.Entity("cardGame.Classes.Admin", b =>
                {
                    b.Property<string>("admin_code")
                        .HasColumnType("nvarchar(450)");

                    b.Property<bool>("admin_state")
                        .HasColumnType("bit");

                    b.Property<string>("player_name")
                        .IsRequired()
                        .HasColumnType("nvarchar(450)");

                    b.HasKey("admin_code");

                    b.HasIndex("player_name")
                        .IsUnique();

                    b.ToTable("Admins");
                });

            modelBuilder.Entity("cardGame.Classes.Card", b =>
                {
                    b.Property<string>("card_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<int>("card_type")
                        .HasColumnType("int");

                    b.Property<int>("civilisation")
                        .HasColumnType("int");

                    b.Property<long>("energy_cost")
                        .HasColumnType("bigint");

                    b.Property<bool>("evolution_creature")
                        .HasColumnType("bit");

                    b.Property<long>("power")
                        .HasColumnType("bigint");

                    b.Property<int>("race")
                        .HasColumnType("int");

                    b.HasKey("card_name");

                    b.ToTable("Cards");
                });

            modelBuilder.Entity("cardGame.Classes.CardAbility", b =>
                {
                    b.Property<string>("ability_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<string>("card_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<long>("quantity")
                        .HasColumnType("bigint");

                    b.HasKey("ability_name", "card_name");

                    b.HasIndex("card_name");

                    b.ToTable("Card_Abilities");
                });

            modelBuilder.Entity("cardGame.Classes.Deck", b =>
                {
                    b.Property<string>("deck_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<bool>("favorite")
                        .HasColumnType("bit");

                    b.Property<string>("player_name")
                        .IsRequired()
                        .HasColumnType("nvarchar(450)");

                    b.Property<long>("tournament_count")
                        .HasColumnType("bigint");

                    b.Property<double>("win_rate")
                        .HasColumnType("float");

                    b.HasKey("deck_name");

                    b.HasIndex("player_name");

                    b.ToTable("Decks");
                });

            modelBuilder.Entity("cardGame.Classes.DeckCard", b =>
                {
                    b.Property<string>("card_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<string>("deck_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<int>("card_count")
                        .HasColumnType("int");

                    b.HasKey("card_name", "deck_name");

                    b.HasIndex("deck_name");

                    b.ToTable("Deck_Cards");
                });

            modelBuilder.Entity("cardGame.Classes.Player", b =>
                {
                    b.Property<string>("player_name")
                        .HasColumnType("nvarchar(450)");

                    b.Property<DateTime>("register_date")
                        .HasColumnType("datetime2");

                    b.Property<double>("win_rate")
                        .HasColumnType("float");

                    b.HasKey("player_name");

                    b.ToTable("Players");
                });

            modelBuilder.Entity("cardGame.Classes.StrategicDetails", b =>
                {
                    b.Property<long>("id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("bigint")
                        .HasAnnotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.None);

                    b.Property<string>("card_name")
                        .IsRequired()
                        .HasColumnType("nvarchar(450)");

                    b.Property<string>("deck_name")
                        .IsRequired()
                        .HasColumnType("nvarchar(450)");

                    b.Property<string>("role")
                        .HasColumnType("nvarchar(max)");

                    b.Property<string>("synergy_details")
                        .HasColumnType("nvarchar(max)");

                    b.HasKey("id");

                    b.HasIndex("card_name", "deck_name")
                        .IsUnique();

                    b.ToTable("Strategic_Details");
                });

            modelBuilder.Entity("cardGame.Classes.Admin", b =>
                {
                    b.HasOne("cardGame.Classes.Player", "player")
                        .WithOne("admin")
                        .HasForeignKey("cardGame.Classes.Admin", "player_name")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("player");
                });

            modelBuilder.Entity("cardGame.Classes.CardAbility", b =>
                {
                    b.HasOne("cardGame.Classes.Ability", "ability")
                        .WithMany("cardAbilities")
                        .HasForeignKey("ability_name")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.HasOne("cardGame.Classes.Card", "card")
                        .WithMany("cardAbilities")
                        .HasForeignKey("card_name")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("ability");

                    b.Navigation("card");
                });

            modelBuilder.Entity("cardGame.Classes.Deck", b =>
                {
                    b.HasOne("cardGame.Classes.Player", "owner")
                        .WithMany("decks")
                        .HasForeignKey("player_name")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("owner");
                });

            modelBuilder.Entity("cardGame.Classes.DeckCard", b =>
                {
                    b.HasOne("cardGame.Classes.Card", "card")
                        .WithMany("deckCards")
                        .HasForeignKey("card_name")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.HasOne("cardGame.Classes.Deck", "deck")
                        .WithMany("deckCards")
                        .HasForeignKey("deck_name")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("card");

                    b.Navigation("deck");
                });

            modelBuilder.Entity("cardGame.Classes.StrategicDetails", b =>
                {
                    b.HasOne("cardGame.Classes.DeckCard", "deckCard")
                        .WithOne("strategicDetails")
                        .HasForeignKey("cardGame.Classes.StrategicDetails", "card_name", "deck_name")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("deckCard");
                });

            modelBuilder.Entity("cardGame.Classes.Ability", b =>
                {
                    b.Navigation("cardAbilities");
                });

            modelBuilder.Entity("cardGame.Classes.Card", b =>
                {
                    b.Navigation("cardAbilities");

                    b.Navigation("deckCards");
                });

            modelBuilder.Entity("cardGame.Classes.Deck", b =>
                {
                    b.Navigation("deckCards");
                });

            modelBuilder.Entity("cardGame.Classes.DeckCard", b =>
                {
                    b.Navigation("strategicDetails");
                });

            modelBuilder.Entity("cardGame.Classes.Player", b =>
                {
                    b.Navigation("admin");

                    b.Navigation("decks");
                });
#pragma warning restore 612, 618
        }
    }
}