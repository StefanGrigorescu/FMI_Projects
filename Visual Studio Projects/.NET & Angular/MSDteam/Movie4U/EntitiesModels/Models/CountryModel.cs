using Movie4U.EntitiesModels.Entities;

namespace Movie4U.EntitiesModels.Models
{
    public class CountryModel : EntitiesModelsBase<Country, CountryModel>
    {
        public string country { get; set; }

        public string country_code { get; set; }  // while getting from uNoGs, it's name might not contain '_' for this class 

        public int expiring { get; set; }

        public int nl7 { get; set; }

        public int tmovs { get; set; }

        public int tseries { get; set; }

        public int tvids { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public CountryModel(CountryModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public CountryModel(Country source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public CountryModel() { }

        override public void Copy(Country source)
        {
            country = source.country;
            country_code = source.country_code;
            expiring = source.expiring;
            nl7 = source.nl7;
            tmovs = source.tmovs;
            tseries = source.tseries;
            tvids = source.tvids;
        }

        override public void Copy(CountryModel source)
        {
            country = source.country;
            country_code = source.country_code;
            expiring = source.expiring;
            nl7 = source.nl7;
            tmovs = source.tmovs;
            tseries = source.tseries;
            tvids = source.tvids;
        }
    }
}
