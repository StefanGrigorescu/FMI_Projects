using Movie4U.EntitiesModels.Entities;

namespace Movie4U.EntitiesModels.Models
{
    public class TitleImageModel : EntitiesModelsBase<TitleImage, TitleImageModel>
    {
        public string image_type { get; set; }
        public string netflix_id { get; set; }
        public string url { get; set; }        // "filmid as netflixid,url,itype"



        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleImageModel(TitleImageModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleImageModel(TitleImage source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleImageModel() { }

        override public void Copy(TitleImage source)
        {
            image_type = source.image_type;
            netflix_id = source.netflix_id;
            url = source.url;
        }

        override public void Copy(TitleImageModel source)
        {
            image_type = source.image_type;
            netflix_id = source.netflix_id;
            url = source.url;
        }
    }
}
