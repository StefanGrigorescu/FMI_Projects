using System;
using Movie4U.EntitiesModels.Entities;

namespace Movie4U.EntitiesModels.Models
{
    public class WatcherTitleModel: EntitiesModelsBase<WatcherTitle,WatcherTitleModel>
    {

        public string watcher_name { get; set; }

        public string netflix_id { get; set; }

        public WatcherTitle.Prefferences prefference { get; set; }

        public DateTime prefLastSetTime { get; set; }

        public bool watchLater { get; set; }

        public DateTime watchLaterLastSetTime { get; set; }

        public TitleModel titleModel { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitleModel(WatcherTitle source) 
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitleModel(WatcherTitleModel source)
        {
            ShallowCopy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitleModel() { }

        override public void Copy(WatcherTitle source)
        {
            watcher_name = source.watcher_name;
            netflix_id = source.netflix_id;
            prefference = source.prefference;
            prefLastSetTime = source.prefLastSetTime;
            watchLater = source.watchLater;
            watchLaterLastSetTime = source.watchLaterLastSetTime;
        }

        override public void Copy(WatcherTitleModel source)
        {
            watcher_name = source.watcher_name;
            netflix_id = source.netflix_id;
            prefference = source.prefference;
            prefLastSetTime = source.prefLastSetTime;
            watchLater = source.watchLater;
            watchLaterLastSetTime = source.watchLaterLastSetTime;
        }

        override public void ShallowCopy(WatcherTitleModel source)
        {
            Copy(source);
            titleModel = source.titleModel;
        }
    }
}
