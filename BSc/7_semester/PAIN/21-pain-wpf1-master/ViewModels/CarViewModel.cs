using CarsWPF.Models;
using CarsWPF.MVVM;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static CarsWPF.Models.Car;

namespace CarsWPF.ViewModels
{
    public class CarViewModel : MVVM.IViewModel, INotifyPropertyChanged 
    {
        private CarsModel CarsModel { get; }
        private Car? Car { get; }
        public Action? Close { get; set; }

        public string Brand { get; set; } = "";
        public bool BrandError { get; set; } = false;
        public int MaxSpeed { get; set; } = 0;
        public bool MaxSpeedError { get; set; } = false;
        public DateTime ProductionDate { get; set; } = DateTime.Now;
        public bool ProductionDateError { get; set; } = false;
        public CarType Type { get; set; } = CarType.Passenger;

        private string errorMessage;

        public event PropertyChangedEventHandler? PropertyChanged;

        public string ErrorMessage
        {
            get => errorMessage;
            set
            {
                errorMessage = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ErrorMessage)));
            }
        }

        public RelayCommand<CarViewModel> OkCommand { get; } = new RelayCommand<CarViewModel>
    (
        (carViewModel) => { carViewModel.Ok(); }
    );
        public RelayCommand<CarViewModel> CancelCommand { get; } = new RelayCommand<CarViewModel>
            (
                (studentViewModel) => { studentViewModel.Cancel(); }
            );

        public CarViewModel(CarsModel carsModel, Car? car)
        {
            CarsModel = carsModel;
            Car = car;
            if (Car != null)
            {
                Brand = Car.Brand;
                MaxSpeed = Car.MaxSpeed;
                ProductionDate = Car.ProductionDate;
                Type = Car.Type;
            }
            ErrorMessage = "";
        }

        private bool Validate()
        {
            ErrorMessage = "";
            foreach (Car car in CarsModel.Cars)
            {
                if (Brand.Equals(car.Brand) && !ReferenceEquals(car, Car))
                {
                    ErrorMessage = "Brand already taken";
                    return false;
                }
            }
            if (BrandError)
                return false;
            if (MaxSpeedError)
                return false;
            if (ProductionDateError)
                return false;
            return true;
        }

        private void Ok()
        {
            if (!Validate())
                return;
            if (Car == null)
            {
                Car car = new Car(Brand, MaxSpeed, ProductionDate, Type);
                CarsModel.Cars.Add(car);
            }
            else
            {
                Car.Brand    = Brand;
                Car.MaxSpeed = MaxSpeed;
                Car.ProductionDate   = ProductionDate;
                Car.Type   = Type;
            }
            Close?.Invoke();
        }
        private void Cancel() => Close?.Invoke();

    }
}
