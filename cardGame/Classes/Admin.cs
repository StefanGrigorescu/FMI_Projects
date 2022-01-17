using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Classes
{
    public class Admin
    {
        /*public Admin(string admin_code, string player_name, bool admin_state, Player player)
        {
            this.admin_code = admin_code ?? throw new ArgumentNullException(nameof(admin_code));
            this.player_name = player_name ?? throw new ArgumentNullException(nameof(player_name));
            this.admin_state = admin_state;
            this.player = player ?? throw new ArgumentNullException(nameof(player));
        }*/

        public static string NewID()
        {
            return Guid.NewGuid().ToString();
        }

        [Required, Key]
        public string admin_code = NewID();

        [Required]
        public string player_name { get; set; }

        public bool admin_state { get; set; }

        public virtual Player player { get; set; }

    }
}
