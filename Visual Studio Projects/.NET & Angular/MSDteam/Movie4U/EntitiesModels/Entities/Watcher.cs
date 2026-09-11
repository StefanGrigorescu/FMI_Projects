using Movie4U.EntitiesModels.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Movie4U.EntitiesModels.Entities
{
    public class Watcher: EntitiesModelsBase<Watcher,WatcherModel>
    {
        [Required, Key]
        public string watcher_name { get; set; }

        public DateTime register_date { get; set; }

        public string refreshToken { get; set; }

        public DateTime refreshTokenExpiryTime { get; set; }

        [Required]
        public string userId { get; set; }

        public virtual User user { get; set; }
        public virtual List<WatcherTitle> watcherTitles { get; set; }
        public virtual List<WatcherGenre> watcherGenres { get; set; }

        /**<summary>
         * Constructor.
         * </summary>*/
        public Watcher(Watcher source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public Watcher(WatcherModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public Watcher() { }

        public override void Copy(Watcher source)
        {
            this.watcher_name = source.watcher_name;
            this.register_date = source.register_date;
            this.refreshToken = source.refreshToken;
            this.refreshTokenExpiryTime = source.refreshTokenExpiryTime;
            this.userId = source.userId;
        }

        public override void Copy(WatcherModel source)
        {
            this.watcher_name = source.watcher_name;
            this.register_date = source.register_date;
            this.refreshToken = source.refreshToken;
            this.refreshTokenExpiryTime = source.refreshTokenExpiryTime;
            this.userId = source.userId;
        }

    }
}
