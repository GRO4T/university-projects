using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarsWPF.Models
{
    public enum CarType
    {
        Passenger,
        Truck,
        Motorcycle
    }

    public class Car : INotifyPropertyChanged
    {

        public event PropertyChangedEventHandler? PropertyChanged;

        private string brand = "";
        public string Brand
        {
            get => brand;
            set
            {
                brand = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Brand)));
            }
        }

        private int maxSpeed = 0;
        public int MaxSpeed
        {
            get => maxSpeed;
            set
            {
                maxSpeed = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(MaxSpeed)));
            }
        }

        private DateTime productionDate = DateTime.Now;
        public DateTime ProductionDate 
        {
            get => productionDate;
            set
            {
                productionDate = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ProductionDate)));
            }
        }

        private CarType carType = CarType.Passenger;
        public CarType Type 
        {
            get => carType;
            set
            {
                carType = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(CarType)));
            }
        }

        public Car(string brand, int maxSpeed, DateTime productionDate, CarType type)
        {
            Brand = brand;
            MaxSpeed = maxSpeed;
            ProductionDate = productionDate;
            Type = type;
        }
    }
}
