using Movie4U.EntitiesModels.Entities;

namespace Movie4U.EntitiesModels.Models
{
    public class GenreModel: EntitiesModelsBase<Genre, GenreModel>
    {
        public string genre { get; set; }

        public int genre_id { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public GenreModel(GenreModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public GenreModel(Genre source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public GenreModel() { }

        override public void Copy(Genre source)
        {
            genre = source.genre;
            genre_id = source.genre_id;
        }

        override public void Copy(GenreModel source)
        {
            genre = source.genre;
            genre_id = source.genre_id;
        }
    }
}
