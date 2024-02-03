using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace CarsWPF.MVVM
{
	public class RelayCommand<T> : ICommand
	{
		private readonly Action<T> _execute;
		private readonly Predicate<T>? _canExecute = null;
		public event EventHandler? CanExecuteChanged;
		public RelayCommand(Action<T> execute)
		{
			_execute = execute;
		}
		public RelayCommand(Action<T> execute, Predicate<T> canExecute)
		{
			_execute = execute ?? throw new ArgumentNullException(nameof(execute));
			_canExecute = canExecute;
		}
		public bool CanExecute(object? parameter)
		{
			if (_canExecute is null)
				return true;
			if (parameter is null)
				throw new ArgumentNullException(nameof(parameter));
			return _canExecute.Invoke((T)parameter);
		}
		public void Execute(object? parameter)
		{
			if (parameter is null)
				throw new ArgumentNullException(nameof(parameter));
			_execute((T)parameter);
		}
		public void NotifyCanExecuteChanged()
		{
			CanExecuteChanged?.Invoke(this, new EventArgs());
		}
	}
}