using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class CardAbility
    {
        /*public CardAbility(string ability_name, string card_name, Ability ability, Card card, uint quantity)
        {
            this.ability_name = ability_name ?? throw new ArgumentNullException(nameof(ability_name));
            this.card_name = card_name ?? throw new ArgumentNullException(nameof(card_name));
            this.quantity = quantity;
            this.ability = ability ?? throw new ArgumentNullException(nameof(ability));
            this.card = card ?? throw new ArgumentNullException(nameof(card));
        }*/

        [Required]
        public string ability_name { get; set; }

        [Required]
        public string card_name { get; set; }

        public uint quantity { get; set; }

        public virtual Ability ability { get; set; }

        public virtual Card card { get; set; }
    }
}
