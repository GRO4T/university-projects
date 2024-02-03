using CarsWPF.Models;
using CarsWPF.MVVM;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace CarsWPF.ViewModels
{
    public class CarsViewModel : IViewModel, INotifyPropertyChanged
    {
        private CarsModel CarsModel { get; }
        private readonly CollectionViewSource collectionViewSource;

        public event PropertyChangedEventHandler? PropertyChanged;

        public ICollectionView Cars { get; }

        private Car? selectedCar;

        public Car? SelectedCar
        {
            get { return selectedCar; }
            set
            {
                selectedCar = value;
                EditCommand?.NotifyCanExecuteChanged();
                DeleteCommand?.NotifyCanExecuteChanged();
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(SelectedCar)));
            }
        }

        public int Count
        {
            get => Cars.Cast<object>().Count();
        }

        private ObservableCollection<Filter> maxSpeedFilters;
        public ObservableCollection<Filter> MaxSpeedFilters
        {
            get => maxSpeedFilters;
            set { maxSpeedFilters = value; }
        }
        private Filter filterMaxSpeed;
        public Filter FilterMaxSpeed
        {
            get => filterMaxSpeed;
            set
            {
                filterMaxSpeed = value;
                UpdateFilter();
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(FilterMaxSpeed)));
            }
        }
        
        private void UpdateFilter()
        {
            collectionViewSource.View.Refresh();
        }

        bool FilterCar(Car car)
        {
            return FilterMaxSpeed.Condition(car.MaxSpeed);
        }

        public Action? Close { get; set; }

        public RelayCommand<object> AddCommand { get; }
        public RelayCommand<object> EditCommand { get; }
        public RelayCommand<object> DeleteCommand { get; }
        public RelayCommand<object> NewWindowCommand { get; }

        public CarsViewModel(CarsModel carsModel)
        {
            AddCommand = new RelayCommand<object>(o => AddCar());
            EditCommand = new RelayCommand<object>(o => EditCar(SelectedCar), o => SelectedCar != null);
            DeleteCommand = new RelayCommand<object>(o => DeleteCar(SelectedCar), o => SelectedCar != null);
            NewWindowCommand = new RelayCommand<object>(o => NewWindow());

            CarsModel = carsModel;
            collectionViewSource = new CollectionViewSource
            {
                Source = CarsModel.Cars
            };

            MaxSpeedFilters = new ObservableCollection<Filter>()
            {
                new Filter("All", (o) => true),
                new Filter("Faster than 100 km/h", (o) => Convert.ToInt32(o) >= 100),
                new Filter("Slower than 100 km/h", (o) => Convert.ToInt32(o) < 100)
            };
            FilterMaxSpeed = MaxSpeedFilters[0];

            collectionViewSource.LiveFilteringProperties.Add(nameof(Car.MaxSpeed));
            collectionViewSource.IsLiveFilteringRequested = true;
            collectionViewSource.View.Filter = (o) => FilterCar((Car)o);
            Cars = collectionViewSource.View;
            Cars.CollectionChanged += CarsChanged;

        }

        private void CarsChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Count)));
        }

        private void NewWindow()
        {
            CarsViewModel carsViewModel = new CarsViewModel(CarsModel);
            ((App)Application.Current).WindowService.Show(carsViewModel);
        }

        private void AddCar()
        {
            CarViewModel carViewModel = new CarViewModel(CarsModel, null);
            ((App)Application.Current).WindowService.ShowDialog(carViewModel);
        }

        private void EditCar(Car? car)
        {
            if (car != null)
            {
                CarViewModel carViewModel = new CarViewModel(CarsModel, car);
                ((App)Application.Current).WindowService.ShowDialog(carViewModel);
            }
        }

        private void DeleteCar(Car? car)
        {
            if (car != null)
                CarsModel.Cars.Remove(car);
        }
    }
}
