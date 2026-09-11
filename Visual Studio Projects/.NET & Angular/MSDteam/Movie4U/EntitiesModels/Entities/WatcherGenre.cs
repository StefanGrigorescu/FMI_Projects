using Movie4U.EntitiesModels.Models;
using System.ComponentModel.DataAnnotations;

namespace Movie4U.EntitiesModels.Entities
{
    public class WatcherGenre: EntitiesModelsBase<WatcherGenre, WatcherGenreModel>
    {
        [Required]
        public string watcher_name { get; set; }

        [Required]
        public int genre_id { get; set; }

        public double watcherGenreScore { get; set; }

        virtual public Watcher watcher { get; set; }
        virtual public Genre genre { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherGenre(WatcherGenre source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherGenre(WatcherGenreModel source)
        {
            Copy(source);
        }

        public WatcherGenre(WatcherGenreModelParameter source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherGenre() { }

        override public void Copy(WatcherGenre source)
        {
            watcher_name = source.watcher_name;
            genre_id = source.genre_id;
            watcherGenreScore = source.watcherGenreScore;
        }

        override public void Copy(WatcherGenreModel source)
        {
            watcher_name = source.watcher_name;
            genre_id = source.genre_id;
            watcherGenreScore = source.watcherGenreScore;
        }

        public void Copy(WatcherGenreModelParameter source)
        {
            watcher_name = source.watcher_name;
            genre_id = source.genre_id;
            watcherGenreScore = source.watcherGenreScore;
        }

    }
}
