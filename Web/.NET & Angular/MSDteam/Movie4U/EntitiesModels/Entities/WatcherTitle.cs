using Movie4U.EntitiesModels.Models;
using System;
using System.ComponentModel.DataAnnotations;

namespace Movie4U.EntitiesModels.Entities
{
    public class WatcherTitle: EntitiesModelsBase<WatcherTitle,WatcherTitleModel>
    {
        public enum Prefferences
        {
            Null = 0,
            More = 1,
            Less = 2
        };


        [Required]
        public string watcher_name { get; set; }

        [Required]
        public string netflix_id { get; set; }

        public Prefferences prefference { get; set; }

        public DateTime prefLastSetTime { get; set; }

        public bool watchLater { get; set; }

        public DateTime watchLaterLastSetTime { get; set; }

        virtual public Watcher watcher { get; set; }
        virtual public Title title { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitle(WatcherTitle source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitle(WatcherTitleModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitle(WatcherTitleModelParameter source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherTitle() { }

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
        
        public void Copy(WatcherTitleModelParameter source)
        {
            watcher_name = source.watcher_name;
            netflix_id = source.netflix_id;
            prefference = source.prefference;
            prefLastSetTime = source.prefLastSetTime;
            watchLater = source.watchLater;
            watchLaterLastSetTime = source.watchLaterLastSetTime;
        }

    }
}
