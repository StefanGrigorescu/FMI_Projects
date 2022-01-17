using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class StrategicDetails
    {
        /*public StrategicDetails(uint id, string card_name, string deck_name, string role, List<string> synergy_details, DeckCard deckCard)
        {
            this.id = id;
            this.card_name = card_name ?? throw new ArgumentNullException(nameof(card_name));
            this.deck_name = deck_name ?? throw new ArgumentNullException(nameof(deck_name));
            this.role = role ?? throw new ArgumentNullException(nameof(role));
            this.synergy_details = synergy_details ?? throw new ArgumentNullException(nameof(synergy_details));
            this.deckCard = deckCard ?? throw new ArgumentNullException(nameof(deckCard));
        }*/

        [Required, Key]
        public uint id { get; set; }

        [Required]
        public string card_name { get; set; }

        [Required]
        public string deck_name { get; set; }

        public string role { get; set; }

        public string synergy_details { get; set; }

        public virtual DeckCard deckCard { get; set; }
    }
}
