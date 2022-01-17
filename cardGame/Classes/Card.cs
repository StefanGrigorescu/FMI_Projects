using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;


namespace cardGame.Classes
{
    public class Card
    {
        /*public Card(string card_name, CardTypes card_type, Civilisations civilisation, Races race, uint energy_cost, uint power, bool evolutionCreature, ICollection<DeckCard> deckCards, ICollection<CardAbility> cardAbilities)
        {
            this.card_name = card_name ?? throw new ArgumentNullException(nameof(card_name));
            this.card_type = card_type;
            this.civilisation = civilisation;
            this.race = race;
            this.energy_cost = energy_cost;
            this.power = power;
            this.evolutionCreature = evolutionCreature;
            this.deckCards = deckCards ?? throw new ArgumentNullException(nameof(deckCards));
            this.cardAbilities = cardAbilities ?? throw new ArgumentNullException(nameof(cardAbilities));
        }*/

        [Required, Key]
        public string card_name { get; set; }

        [Required]
        public CardTypes card_type { get; set; }

        [Required]
        public Civilisations civilisation { get; set; }

        public Races race { get; set; }

        [Required, Range(1,15)]
        public uint energy_cost { get; set; }

        [Range (0,20)]
        public uint power { get; set; }

        public bool evolution_creature { get; set; }

        public Sets set_name { get; set; }

        public virtual ICollection<DeckCard> deckCards { get; set; }

        public virtual ICollection<CardAbility> cardAbilities { get; set; }
    }
}
