using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace Movie4U.Migrations
{
    public partial class AddedUnogsEntities : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Countries",
                columns: table => new
                {
                    country_code = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    country = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    expiring = table.Column<int>(type: "int", nullable: false),
                    nl7 = table.Column<int>(type: "int", nullable: false),
                    tmovs = table.Column<int>(type: "int", nullable: false),
                    tseries = table.Column<int>(type: "int", nullable: false),
                    tvids = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Countries", x => x.country_code);
                });

            migrationBuilder.CreateTable(
                name: "Genres",
                columns: table => new
                {
                    genre_id = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    genre = table.Column<string>(type: "nvarchar(max)", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Genres", x => x.genre_id);
                });

            migrationBuilder.CreateTable(
                name: "Title_Details",
                columns: table => new
                {
                    netflix_id = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    alt_id = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    alt_image = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    alt_metascore = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    alt_plot = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    alt_runtime = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    alt_votes = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    awards = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    default_image = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    large_image = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    latest_date = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    maturity_label = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    maturity_level = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    origin_country = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    poster = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    rating = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    runtime = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    start_date = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    synopsis = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    title = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    title_type = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    year = table.Column<string>(type: "nvarchar(max)", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Title_Details", x => x.netflix_id);
                });

            migrationBuilder.CreateTable(
                name: "Title_Countries",
                columns: table => new
                {
                    country_code = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    netflix_id = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    audio = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    country = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    expire_date = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    new_date = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    season_detail = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    subtitle = table.Column<string>(type: "nvarchar(max)", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Title_Countries", x => new { x.netflix_id, x.country_code });
                    table.ForeignKey(
                        name: "FK_Title_Countries_Countries_country_code",
                        column: x => x.country_code,
                        principalTable: "Countries",
                        principalColumn: "country_code",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_Title_Countries_Title_Details_netflix_id",
                        column: x => x.netflix_id,
                        principalTable: "Title_Details",
                        principalColumn: "netflix_id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Title_Genres",
                columns: table => new
                {
                    genre_id = table.Column<int>(type: "int", nullable: false),
                    netflix_id = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    genre = table.Column<string>(type: "nvarchar(max)", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Title_Genres", x => new { x.netflix_id, x.genre_id });
                    table.ForeignKey(
                        name: "FK_Title_Genres_Genres_genre_id",
                        column: x => x.genre_id,
                        principalTable: "Genres",
                        principalColumn: "genre_id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_Title_Genres_Title_Details_netflix_id",
                        column: x => x.netflix_id,
                        principalTable: "Title_Details",
                        principalColumn: "netflix_id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Title_Images",
                columns: table => new
                {
                    url = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    image_type = table.Column<string>(type: "nvarchar(max)", nullable: true),
                    netflix_id = table.Column<string>(type: "nvarchar(450)", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Title_Images", x => x.url);
                    table.ForeignKey(
                        name: "FK_Title_Images_Title_Details_netflix_id",
                        column: x => x.netflix_id,
                        principalTable: "Title_Details",
                        principalColumn: "netflix_id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "Watcher_Titles",
                columns: table => new
                {
                    watcher_name = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    netflix_id = table.Column<string>(type: "nvarchar(450)", nullable: false),
                    prefference = table.Column<int>(type: "int", nullable: false),
                    prefLastSetTime = table.Column<DateTime>(type: "datetime2", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Watcher_Titles", x => new { x.watcher_name, x.netflix_id });
                    table.ForeignKey(
                        name: "FK_Watcher_Titles_Title_Details_netflix_id",
                        column: x => x.netflix_id,
                        principalTable: "Title_Details",
                        principalColumn: "netflix_id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_Watcher_Titles_Watchers_watcher_name",
                        column: x => x.watcher_name,
                        principalTable: "Watchers",
                        principalColumn: "watcher_name",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_Title_Countries_country_code",
                table: "Title_Countries",
                column: "country_code");

            migrationBuilder.CreateIndex(
                name: "IX_Title_Genres_genre_id",
                table: "Title_Genres",
                column: "genre_id");

            migrationBuilder.CreateIndex(
                name: "IX_Title_Images_netflix_id",
                table: "Title_Images",
                column: "netflix_id");

            migrationBuilder.CreateIndex(
                name: "IX_Watcher_Titles_netflix_id",
                table: "Watcher_Titles",
                column: "netflix_id");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Title_Countries");

            migrationBuilder.DropTable(
                name: "Title_Genres");

            migrationBuilder.DropTable(
                name: "Title_Images");

            migrationBuilder.DropTable(
                name: "Watcher_Titles");

            migrationBuilder.DropTable(
                name: "Countries");

            migrationBuilder.DropTable(
                name: "Genres");

            migrationBuilder.DropTable(
                name: "Title_Details");
        }
    }
}
