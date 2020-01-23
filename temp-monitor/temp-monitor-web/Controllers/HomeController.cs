using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Temp.Monitor.Web.Models;

namespace Temp.Monitor.Web.Controllers
{
    public class HomeController : Controller
    {
        public IActionResult Index()
        {
            var tempData = GetData();

            return View(tempData);
        }


        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }


        public List<TemperatureItemModel> GetData()
        {
            List<TemperatureItemModel> data;
            string url = "https://shaun-temp-mon.azurewebsites.net/api/temperature";
            string functionKey = "5DluXPfgBx6S2O1/zNgV/VMXu1BAgk2asWMqXYb4Sp/628XOQyBJoA==";

            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
            req.Headers.Add("Content-type", "application/json");
            req.Headers.Add("x-functions-key", functionKey);

            WebResponse res = req.GetResponse();

            using (StreamReader reader = new StreamReader(res.GetResponseStream()))
            {
                string result = reader.ReadToEnd();
                data = Newtonsoft.Json.JsonConvert.DeserializeObject<List<TemperatureItemModel>>(result);
            }

            string nzTimeZoneKey = "New Zealand Standard Time";
            TimeZoneInfo nzTimeZone = TimeZoneInfo.FindSystemTimeZoneById(nzTimeZoneKey);

            //get most recent 300 items :D
            data = data.OrderBy(x => x.TempDate).TakeLast(300).ToList();

            data.ForEach(item => { 
                item.TempDate = TimeZoneInfo.ConvertTimeFromUtc(item.TempDate, nzTimeZone);
            });



            return data;
        }
    }
}
