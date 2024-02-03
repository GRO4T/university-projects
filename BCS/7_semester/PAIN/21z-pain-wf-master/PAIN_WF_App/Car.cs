using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PAIN_WF_App
{
    public class Car
    {
        public enum CarType
        {
            Passenger,
            Truck,
            Motorcycle
        }

        public Car(string brand, int maxSpeed, DateTime productionDate, CarType type)
        {
            Brand = brand;
            MaxSpeed = maxSpeed;
            ProductionDate = productionDate;
            Type = type;
        }

        public string Brand { get; set; }
        public int MaxSpeed { get; set; }
        public DateTime ProductionDate { get; set; } 
        public CarType Type { get; set; } 
    }
}
