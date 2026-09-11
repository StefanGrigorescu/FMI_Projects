using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace Movie4U.Migrations
{
    public partial class AddedWatcherGenres : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "genre",
                table: "Title_Genres");

            migrationBuilder.DropColumn(
                name: "audio",
                table: "Title_Countries");

            migrationBuilder.DropColumn(
                name: "country",
                table: "Title_Countries");

            migrationBuilder.DropColumn(
                name: "expire_date",
                table: "Title_Countries");

            migrationBuilder.DropColumn(
                name: "new_date",
                table: "Title_Countries");

            migrationBuilder.DropColumn(
                name: "season_detail",
                table: "Title_Countries");

            migrationBuilder.DropColumn(
                name: "subtitle",
                table: "Title_Countries");

            migrationBuilder.AddColumn<bool>(
                name: "watchLater",
                table: "Watcher_Titles",
                type: "bit",
                nullable: false,
                defaultValue: false);

            migrationBuilder.AddColumn<DateTime>(
                name: "watchLaterLastSetTime",
                table: "Watcher_Titles",
                type: "datetime2",
                nullable: false,
                defaultValue: new DateTime(1, 1, 1, 0, 0, 0, 0, DateTimeKind.Unspecified));

            migrationBuilder.CreateTable(
                name: "Watcher_Genres",
                columns: table => new
                {
                    watcher_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    genre_id = table.Column<int>(type: "int", nullable: false),
                    watcherGenreScore = table.Column<double>(type: "float", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Watcher_Genres", x => new { x.watcher_name, x.genre_id });
                    table.ForeignKey(
                        name: "FK_Watcher_Genres_Genres_genre_id",
                        column: x => x.genre_id,
                        principalTable: "Genres",
                        principalColumn: "genre_id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_Watcher_Genres_Watchers_watcher_name",
                        column: x => x.watcher_name,
                        principalTable: "Watchers",
                        principalColumn: "watcher_name",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_Watcher_Genres_genre_id",
                table: "Watcher_Genres",
                column: "genre_id");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Watcher_Genres");

            migrationBuilder.DropColumn(
                name: "watchLater",
                table: "Watcher_Titles");

            migrationBuilder.DropColumn(
                name: "watchLaterLastSetTime",
                table: "Watcher_Titles");

            migrationBuilder.AddColumn<string>(
                name: "genre",
                table: "Title_Genres",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "audio",
                table: "Title_Countries",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "country",
                table: "Title_Countries",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "expire_date",
                table: "Title_Countries",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "new_date",
                table: "Title_Countries",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "season_detail",
                table: "Title_Countries",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "subtitle",
                table: "Title_Countries",
                type: "nvarchar(max)",
                nullable: true);
        }
    }
}
