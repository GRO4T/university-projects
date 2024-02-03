using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarsWPF.Models
{
    public class CarsModel
    {
        public ObservableCollection<Car> Cars { get; private set; } = new ObservableCollection<Car>();
        public CarsModel()
        {
            Cars.Add(new Car("BMW", 250, new DateTime(2021, 5, 1, 0, 0, 0), CarType.Passenger));
            Cars.Add(new Car("Audi", 260, new DateTime(2017, 5, 1, 0, 0, 0), CarType.Passenger));
            Cars.Add(new Car("Opel", 220, new DateTime(2008, 5, 1, 0, 0, 0), CarType.Passenger));
        }

    }
}
