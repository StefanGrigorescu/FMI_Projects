using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class Player
    {
        /*public Player(string player_name, DateTime register_date, ICollection<Deck> decks, Admin admin, double win_rate = 0)
        {
            this.player_name = player_name ?? throw new ArgumentNullException(nameof(player_name));
            this.register_date = register_date;
            this.win_rate = win_rate;
            this.decks = decks ?? throw new ArgumentNullException(nameof(decks));
            this.admin = admin ?? throw new ArgumentNullException(nameof(admin));
        }*/
        
        [Required, Key]
        public string player_name { get; set; }

        [Required, Range(5, 25)]
        public string password { get; set; }

        public DateTime register_date { get; set; }

        [Range(0, 1)]
        public double win_rate { get; set; }

        public virtual ICollection<Deck> decks { get; set; }

        public virtual Admin admin { get; set; }
    }
}
