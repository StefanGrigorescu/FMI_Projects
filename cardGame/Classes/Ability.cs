using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class Ability
    {
        /*public Ability(string ability_name, TriggerMoments tigger_moment, TriggerConditions trigger_conditions, string detalii_efect, ICollection<CardAbility> cardAbilities)
        {
            this.ability_name = ability_name ?? throw new ArgumentNullException(nameof(ability_name));
            this.tigger_moment = tigger_moment ?? throw new ArgumentNullException(nameof(tigger_moment));
            this.trigger_conditions = trigger_conditions ?? throw new ArgumentNullException(nameof(trigger_conditions));
            this.detalii_efect = detalii_efect ?? throw new ArgumentNullException(nameof(detalii_efect));
            this.cardAbilities = cardAbilities;
        }*/

        [Required, Key]
        public string ability_name { get; set; }

        public TriggerMoments trigger_moment { get; set; }

        public TriggerConditions trigger_condition { get; set; }

        [Required]
        public string effect_details { get; set; }

        public virtual ICollection<CardAbility> cardAbilities { get; set; }
    }
}
