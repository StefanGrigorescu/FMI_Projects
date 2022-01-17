using Microsoft.EntityFrameworkCore.Migrations;

namespace cardGame.Migrations
{
    public partial class AddedCardSets : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<int>(
                name: "set_name",
                table: "Cards",
                type: "int",
                nullable: false,
                defaultValue: 0);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "set_name",
                table: "Cards");
        }
    }
}
