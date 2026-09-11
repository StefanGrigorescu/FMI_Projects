using Movie4U.EntitiesModels.Entities;

namespace Movie4U.EntitiesModels.Models
{
    public class WatcherGenreModel: EntitiesModelsBase<WatcherGenre, WatcherGenreModel>
    {
        public string watcher_name { get; set; }

        public int genre_id { get; set; }

        public double watcherGenreScore { get; set; }

        public GenreModel genreModel { get; set; }



        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherGenreModel(WatcherGenre source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherGenreModel(WatcherGenreModel source)
        {
            ShallowCopy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public WatcherGenreModel() { }

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

        override public void ShallowCopy(WatcherGenreModel source)
        {
            Copy(source);
            genreModel = source.genreModel;
        }
    }
}
