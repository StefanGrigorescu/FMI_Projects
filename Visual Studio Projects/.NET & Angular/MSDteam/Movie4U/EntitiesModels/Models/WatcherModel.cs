using Movie4U.EntitiesModels.Entities;
using System;

namespace Movie4U.EntitiesModels.Models
{
    public class WatcherModel: EntitiesModelsBase<Watcher,WatcherModel>
    {
        public string watcher_name { get; set; }

        public DateTime register_date { get; set; }

        public string refreshToken { get; set; }

        public DateTime refreshTokenExpiryTime { get; set; }

        public string userId { get; set; }


        /**<summary>
        * Constructor.
        * </summary>*/
        public WatcherModel(WatcherModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherModel(Watcher source)
        {
            Copy(source);
        }
        
        /**<summary>
        * Constructor.
        * </summary>*/
        public WatcherModel() { }

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
