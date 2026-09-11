using Movie4U.EntitiesModels.Models;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Movie4U.EntitiesModels.Entities
{
    public class Title: EntitiesModelsBase<Title,TitleModel>
    {
        public string alt_id { get; set; }

        public string alt_image { get; set; }

        public string alt_metascore { get; set; }

        public string alt_plot { get; set; }

        public string alt_runtime { get; set; }

        public string alt_votes { get; set; }

        public string awards { get; set; }

        public string default_image { get; set; }

        public string large_image { get; set; }

        public string latest_date { get; set; }

        public string maturity_label { get; set; }

        public string maturity_level { get; set; }

        [Required, Key]
        public string netflix_id { get; set; }

        public string origin_country { get; set; }

        public string poster { get; set; }

        public string rating { get; set; }

        public string runtime { get; set; }

        public string start_date { get; set; }

        public string synopsis { get; set; }

        public string title { get; set; }

        public string title_type { get; set; }

        public string year { get; set; }

        virtual public List<TitleCountry> titleCountries { get; set; }
        virtual public List<TitleGenre> titleGenres { get; set; }
        virtual public List<TitleImage> titleImages { get; set; }
        public virtual List<WatcherTitle> watcherTitles { get; set; }


        /**<summary>
         * Constructor.
         * </summary>*/
        public Title(Title source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public Title(TitleModel source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public Title(TitleModelParameter source)
        {
            Copy(source);
        }

        /**<summary>
         * Constructor.
         * </summary>*/
        public Title() { }


        override public void Copy(Title source)
        {
            this.alt_id = source.alt_id;
            this.alt_image = source.alt_image;
            this.alt_metascore = source.alt_metascore;
            this.alt_plot = source.alt_plot;
            this.alt_runtime = source.alt_runtime;
            this.alt_votes = source.alt_votes;
            this.awards = source.awards;
            this.default_image = source.default_image;
            this.large_image = source.large_image;
            this.latest_date = source.latest_date;
            this.maturity_label = source.maturity_label;
            this.maturity_level = source.maturity_level;
            this.netflix_id = source.netflix_id;
            this.origin_country = source.origin_country;
            this.poster = source.poster;
            this.rating = source.rating;
            this.runtime = source.runtime;
            this.start_date = source.start_date;
            this.synopsis = source.synopsis;
            this.title = source.title;
            this.title_type = source.title_type;
            this.year = source.year;
        }

        override public void Copy(TitleModel source)
        {
            this.alt_id = source.alt_id;
            this.alt_image = source.alt_image;
            this.alt_metascore = source.alt_metascore;
            this.alt_plot = source.alt_plot;
            this.alt_runtime = source.alt_runtime;
            this.alt_votes = source.alt_votes;
            this.awards = source.awards;
            this.default_image = source.default_image;
            this.large_image = source.large_image;
            this.latest_date = source.latest_date;
            this.maturity_label = source.maturity_label;
            this.maturity_level = source.maturity_level;
            this.netflix_id = source.netflix_id;
            this.origin_country = source.origin_country;
            this.poster = source.poster;
            this.rating = source.rating;
            this.runtime = source.runtime;
            this.start_date = source.start_date;
            this.synopsis = source.synopsis;
            this.title = source.title;
            this.title_type = source.title_type;
            this.year = source.year;
        }

        public void Copy(TitleModelParameter source)
        {
            this.alt_id = source.alt_id;
            this.alt_image = source.alt_image;
            this.alt_metascore = source.alt_metascore;
            this.alt_plot = source.alt_plot;
            this.alt_runtime = source.alt_runtime;
            this.alt_votes = source.alt_votes;
            this.awards = source.awards;
            this.default_image = source.default_image;
            this.large_image = source.large_image;
            this.latest_date = source.latest_date;
            this.maturity_label = source.maturity_label;
            this.maturity_level = source.maturity_level;
            this.netflix_id = source.netflix_id;
            this.origin_country = source.origin_country;
            this.poster = source.poster;
            this.rating = source.rating;
            this.runtime = source.runtime;
            this.start_date = source.start_date;
            this.synopsis = source.synopsis;
            this.title = source.title;
            this.title_type = source.title_type;
            this.year = source.year;
        }

    }
}
