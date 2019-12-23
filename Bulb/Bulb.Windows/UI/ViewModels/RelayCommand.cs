using System;
using System.Windows.Input;

namespace Bulb.UI {
	public class RelayCommand : ICommand {
		public event EventHandler CanExecuteChanged = (sender, e) => { };

		private Action action;

		public RelayCommand(Action action) {
			this.action = action;
		}

		public bool CanExecute(object parameter) {
			return true;
		}

		public void Execute(object parameter) {
			action();
		}
	}
}