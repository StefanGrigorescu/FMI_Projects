using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class DeckCard
    {
        /*public DeckCard(string card_name, string deck_name, int card_count, Card card, Deck deck, StrategicDetails strategicDetails)
        {
            this.card_name = card_name ?? throw new ArgumentNullException(nameof(card_name));
            this.deck_name = deck_name ?? throw new ArgumentNullException(nameof(deck_name));
            this.card_count = card_count;
            this.card = card ?? throw new ArgumentNullException(nameof(card));
            this.deck = deck ?? throw new ArgumentNullException(nameof(deck));
            this.strategicDetails = strategicDetails ?? throw new ArgumentNullException(nameof(strategicDetails));
        }*/

        [Required]
        public string card_name { get; set; }

        [Required]
        public string deck_name { get; set; }

        [Range(1, 4)]
        public uint card_count { get; set; }

        public virtual Card card { get; set; }

        public virtual Deck deck { get; set; }

        public virtual StrategicDetails strategicDetails { get; set; }
    }
}
