using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Temp.Monitor.Web.Models
{
    public class TemperatureItemModel
    {
        public float TempInside { get; set; }
        public float TempOutside { get; set; }

        public float Humidity { get; set; }
        public DateTime TempDate { get; set; }
    
    }

}
