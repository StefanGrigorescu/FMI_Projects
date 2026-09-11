using Movie4U.EntitiesModels.Entities;

namespace Movie4U.EntitiesModels.Models
{
    public class TitleCountryModel: EntitiesModelsBase<TitleCountry, TitleCountryModel>
    {
        public string country_code { get; set; }  // while getting from uNoGs, it's name might not contain '_' for this class 

        public string netflix_id { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleCountryModel(TitleCountry source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleCountryModel(TitleCountryModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleCountryModel() { }

        override public void Copy(TitleCountry source)
        {
            country_code = source.country_code;
            netflix_id = source.netflix_id;
        }

        override public void Copy(TitleCountryModel source)
        {
            country_code = source.country_code;
            netflix_id = source.netflix_id;
        }

    }
}
