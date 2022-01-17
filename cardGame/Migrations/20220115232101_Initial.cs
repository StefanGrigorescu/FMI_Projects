using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace cardGame.Migrations
{
    public partial class Initial : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Abilities",
                columns: table => new
                {
                    ability_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    trigger_moment = table.Column<int>(type: "int", nullable: false),
                    trigger_condition = table.Column<int>(type: "int", nullable: false),
                    effect_details = table.Column<string>(type: "nvarchar(max)", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Abilities", x => x.ability_name);
                });

            migrationBuilder.CreateTable(
                name: "Cards",
                columns: table => new
                {
                    card_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    card_type = table.Column<int>(type: "int", nullable: false),
                    civilisation = table.Column<int>(type: "int", nullable: false),
                    race = table.Column<int>(type: "int", nullable: false),
                    energy_cost = table.Column<long>(type: "bigint", nullable: false),
                    power = table.Column<long>(type: "bigint", nullable: false),
                    evolution_creature = table.Column<bool>(type: "bit", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Cards", x => x.card_name);
                });

            migrationBuilder.CreateTable(
                name: "Players",
                columns: table => new
                {
                    player_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    register_date = table.Column<DateTime>(type: "datetime2", nullable: false),
                    win_rate = table.Column<double>(type: "float", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Players", x => x.player_name);
                });

            migrationBuilder.CreateTable(
                name: "Card_Abilities",
                columns: table => new
                {
                    ability_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    card_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    quantity = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Card_Abilities", x => new { x.ability_name, x.card_name });
                    table.ForeignKey(
                        name: "FK_Card_Abilities_Abilities_ability_name",
                        column: x => x.ability_name,
                        principalTable: "Abilities",
                        principalColumn: "ability_name",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_Card_Abilities_Cards_card_name",
                        column: x => x.card_name,
                        principalTable: "Cards",
                        principalColumn: "card_name",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Admins",
                columns: table => new
                {
                    admin_code = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    player_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    admin_state = table.Column<bool>(type: "bit", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Admins", x => x.admin_code);
                    table.ForeignKey(
                        name: "FK_Admins_Players_player_name",
                        column: x => x.player_name,
                        principalTable: "Players",
                        principalColumn: "player_name",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Decks",
                columns: table => new
                {
                    deck_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    player_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    win_rate = table.Column<double>(type: "float", nullable: false),
                    tournament_count = table.Column<long>(type: "bigint", nullable: false),
                    favorite = table.Column<bool>(type: "bit", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Decks", x => x.deck_name);
                    table.ForeignKey(
                        name: "FK_Decks_Players_player_name",
                        column: x => x.player_name,
                        principalTable: "Players",
                        principalColumn: "player_name",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Deck_Cards",
                columns: table => new
                {
                    card_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    deck_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    card_count = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Deck_Cards", x => new { x.card_name, x.deck_name });
                    table.ForeignKey(
                        name: "FK_Deck_Cards_Cards_card_name",
                        column: x => x.card_name,
                        principalTable: "Cards",
                        principalColumn: "card_name",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_Deck_Cards_Decks_deck_name",
                        column: x => x.deck_name,
                        principalTable: "Decks",
                        principalColumn: "deck_name",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Strategic_Details",
                columns: table => new
                {
                    id = table.Column<long>(type: "bigint", nullable: false),
                    card_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    deck_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    role = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    synergy_details = table.Column<string>(type: "nvarchar(max)", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Strategic_Details", x => x.id);
                    table.ForeignKey(
                        name: "FK_Strategic_Details_Deck_Cards_card_name_deck_name",
                        columns: x => new { x.card_name, x.deck_name },
                        principalTable: "Deck_Cards",
                        principalColumns: new[] { "card_name", "deck_name" },
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_Admins_player_name",
                table: "Admins",
                column: "player_name",
                unique: true);

            migrationBuilder.CreateIndex(
                name: "IX_Card_Abilities_card_name",
                table: "Card_Abilities",
                column: "card_name");

            migrationBuilder.CreateIndex(
                name: "IX_Deck_Cards_deck_name",
                table: "Deck_Cards",
                column: "deck_name");

            migrationBuilder.CreateIndex(
                name: "IX_Decks_player_name",
                table: "Decks",
                column: "player_name");

            migrationBuilder.CreateIndex(
                name: "IX_Strategic_Details_card_name_deck_name",
                table: "Strategic_Details",
                columns: new[] { "card_name", "deck_name" },
                unique: true);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Admins");

            migrationBuilder.DropTable(
                name: "Card_Abilities");

            migrationBuilder.DropTable(
                name: "Strategic_Details");

            migrationBuilder.DropTable(
                name: "Abilities");

            migrationBuilder.DropTable(
                name: "Deck_Cards");

            migrationBuilder.DropTable(
                name: "Cards");

            migrationBuilder.DropTable(
                name: "Decks");

            migrationBuilder.DropTable(
                name: "Players");
        }
    }
}
