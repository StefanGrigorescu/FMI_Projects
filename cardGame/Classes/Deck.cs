using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class Deck
    {
        /*public Deck(string deck_name, string player_name, Player owner, double win_rate = 0, uint tournament_count = 0, bool favorite = false)
        {
            this.deck_name = deck_name ?? throw new ArgumentNullException(nameof(deck_name));
            this.player_name = player_name ?? throw new ArgumentNullException(nameof(player_name));
            this.win_rate = win_rate;
            this.tournament_count = tournament_count;
            this.favorite = favorite;
            this.owner = owner ?? throw new ArgumentNullException(nameof(owner));
        }*/

        [Required, Key]
        public string deck_name { get; set; }

        [Required]
        public string player_name { get; set; }

        [Range(0,1)]
        public double win_rate { get; set; }

        [Range(0, int.MaxValue)]
        public uint tournament_count { get; set; }

        public bool favorite { get; set; }

        public virtual Player owner { get; set; }

        public virtual ICollection<DeckCard> deckCards { get; set; }
    }
}
