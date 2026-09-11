using Movie4U.EntitiesModels.Models;
using System.ComponentModel.DataAnnotations;

namespace Movie4U.EntitiesModels.Entities
{
    public class TitleCountry: EntitiesModelsBase<TitleCountry, TitleCountryModel>
    {
        [Required]
        public string country_code { get; set; }

        [Required]
        public string netflix_id { get; set; }

        virtual public Country Country { get; set; }
        virtual public Title title { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleCountry(TitleCountry source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleCountry(TitleCountryModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public TitleCountry() { }

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
