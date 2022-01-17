using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace cardGame.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class WeatherForecastController : ControllerBase
    {
        private static readonly List<string> Summaries = new List<string>
        {
            "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
        };

        private readonly ILogger<WeatherForecastController> _logger;

        public WeatherForecastController(ILogger<WeatherForecastController> logger)
        {
            _logger = logger;
        }

        [HttpGet]
        public IEnumerable<WeatherForecast> Get()
        {
            var rng = new Random();
            return Enumerable.Range(1, 5).Select(index => new WeatherForecast
            {
                Date = DateTime.Now.AddDays(index),
                TemperatureC = rng.Next(-20, 55),
                Summary = Summaries[rng.Next(Summaries.Count)]
            })
            .ToArray();
        }

        [HttpGet("dezambiguizareRuta/{summaryParam}")]
        public async Task<IActionResult> GetWithTask([FromRoute] string summaryParam)
        {
            var rng = new Random();
            var list = Enumerable.Range(1, 5).Select(index => new WeatherForecast
            {
                Date = DateTime.Now.AddDays(index),
                TemperatureC = rng.Next(-20, 55),
                Summary = Summaries[rng.Next(Summaries.Count)]
            }).ToList();
            var wantedForectast = list.Where(x => x.Summary == summaryParam).ToList();

            if (wantedForectast.Count() == 0)
                return NotFound($"mesaj generic pentru not found summary {summaryParam}");  // dolarul este pt a arata ca introduc intre acolade un parametru 
            return Ok(wantedForectast);
        }

        [HttpPost]
        public async Task<IActionResult> AddAnotherSummary([FromBody] string summaryParam)
        {
            Summaries.Add(summaryParam);

            var rng = new Random();
            var list = Enumerable.Range(1, 5).Select(index => new WeatherForecast
            {
                Date = DateTime.Now.AddDays(index),
                TemperatureC = rng.Next(-20, 55),
                Summary = Summaries[rng.Next(Summaries.Count)]
            }).ToList();
            var wantedForectast = list.Where(x => x.Summary == summaryParam).ToList();

            if (wantedForectast.Count() == 0)
                return NotFound($"mesaj generic pentru not found summary {summaryParam}");  // dolarul este pt a arata ca introduc intre acolade un parametru 
            return Ok(wantedForectast);
        }
    
        [HttpPut]
        public async Task<IActionResult> UpdateSummary([FromBody] string summaryToUpdate)
        {
            var desiredSummary = Summaries.FirstOrDefault(summaryVar => summaryVar == summaryToUpdate);
            if (desiredSummary == null)
            {
                return NotFound("NotFound1: nu s-a gasit in lista summary-ul numit!");
            }
            else
            {
                Summaries.Remove(desiredSummary);
                desiredSummary = "Unavailable";
                Summaries.Add(desiredSummary);
            }
            var rng = new Random();
            var list = Enumerable.Range(1, 20).Select(index => new WeatherForecast
            {
                Date = DateTime.Now.AddDays(index),
                TemperatureC = rng.Next(-20, 55),
                Summary = Summaries[rng.Next(Summaries.Count)]
            }).ToList();

            var wantedForecast = list.Where(x => x.Summary == desiredSummary).ToList();
            if(wantedForecast.Count == 0)
            {
                return NotFound("NotFound2: rng a esuat");
            }
            return Ok(wantedForecast);
        }
        
    }
}
