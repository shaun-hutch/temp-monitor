using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System.Data.SqlClient;
using System.Collections.Generic;

namespace TestFunction
{
    public static class Function1
    {
        [FunctionName("Temperature")]
        public static async Task<IActionResult> Run([HttpTrigger(AuthorizationLevel.Function, new string[] { "post", "get" }, Route = null)] HttpRequest req, ILogger log)
        {
            TempData data;
            int result = 0;
            string conString = Environment.GetEnvironmentVariable("ConnectionString");
            List<TempData> dataList;
            if (string.IsNullOrEmpty(conString))
            {
                log.LogError("Connection string not added to settings.");
                return new StatusCodeResult(500);
            }

            if (req.Method == HttpMethods.Post)
            {
                try
                {
                    using (StreamReader sr = new StreamReader(req.Body))
                    {
                        data = JsonConvert.DeserializeObject<TempData>(await sr.ReadToEndAsync());
                    }
                }
                catch (Exception ex)
                {
                    log.LogError(ex.Message);
                    return new BadRequestResult();
                }

                try
                {
                    using (SqlConnection con = new SqlConnection(conString))
                    {
                        con.Open();
                        SqlCommand cmd = con.CreateCommand();
                        cmd.CommandText = $"INSERT INTO dbo.Temperature (tempinside, tempoutside, pressure) VALUES ({data.TempInside}, {data.TempOutside}, {data.Pressure})";
                        result = cmd.ExecuteNonQuery();
                    }
                }
                catch (Exception ex)
                {
                    log.LogError(ex.Message);
                    return new BadRequestResult();
                }

                if (result > 0)
                {
                    log.LogInformation("Insert success");
                    return new OkResult();
                }
                else
                {
                    log.LogError("Insert Failed");
                    return new BadRequestResult();
                }
            }

            else if (req.Method == HttpMethods.Get)
            {
                using (SqlConnection con = new SqlConnection(conString))
                {
                    con.Open();
                    SqlCommand cmd = con.CreateCommand();
                    cmd.CommandText = "SELECT * FROM dbo.Temperature";
                    using (SqlDataReader dr = cmd.ExecuteReader())
                    {
                        dataList = new List<TempData>();
                        while (dr.Read())
                        {
                            dataList.Add(new TempData
                            {
                                TempInside = float.Parse(dr["tempinside"].ToString()),
                                TempOutside = float.Parse(dr["tempoutside"].ToString()),
                                Pressure = float.Parse(dr["pressure"].ToString()),
                                TempDate = DateTime.Parse(dr["tempdate"].ToString())
                            });
                        }
                    }
                    return new JsonResult(dataList);
                }
            }
            else return new UnauthorizedResult();
        }


        public class TempData
        {
            public float TempInside { get; set; }
            public float TempOutside { get; set; }
            public float Pressure { get; set; }
            public DateTime TempDate { get; set; }
        }
    }
}
