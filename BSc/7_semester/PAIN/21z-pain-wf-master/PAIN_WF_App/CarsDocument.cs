using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PAIN_WF_App
{
    public class CarsDocument
    {
        public List<Car> Cars = new List<Car>();

        public event Action<Car> AddCarEvent;

        public void AddCar(Car car)
        {
            Cars.Add(car);
            AddCarEvent?.Invoke(car);
        }

        public event Action<Car> UpdateCarEvent;

        public void UpdateCar(Car updateCar)
        {
            UpdateCarEvent?.Invoke(updateCar);
        }

        public event Action<Car> DeleteCarEvent;

        public void DeleteCar(Car carToDelete)
        {
            Cars.Remove(carToDelete);
            DeleteCarEvent?.Invoke(carToDelete);
        }
    }
}
