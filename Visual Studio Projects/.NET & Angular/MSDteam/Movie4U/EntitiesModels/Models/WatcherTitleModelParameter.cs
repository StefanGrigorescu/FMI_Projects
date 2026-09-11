using Movie4U.EntitiesModels.Entities;
using System;

namespace Movie4U.EntitiesModels.Models
{
    /**<summary>
     * A WatcherTitleModel without models lists. 
     * Used as parameter for POST and PUT requests.
     * </summary> */
    public class WatcherTitleModelParameter
    {
        public string watcher_name { get; set; }

        public string netflix_id { get; set; }

        public WatcherTitle.Prefferences prefference { get; set; }

        public DateTime prefLastSetTime { get; set; }

        public bool watchLater { get; set; }

        public DateTime watchLaterLastSetTime { get; set; }

    }
}
