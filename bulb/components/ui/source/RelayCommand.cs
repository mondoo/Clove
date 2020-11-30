using System;
using System.Windows.Input;

namespace Garlic.Bulb
{
    /// <summary>
    /// Simple RelayCommand that invokes an action when executed.
    /// </summary>
    public class RelayCommand : ICommand
    {
        public event EventHandler CanExecuteChanged = (sender, e) => { };

        private Action action;

        public RelayCommand(Action action) => this.action = action;

        public bool CanExecute(object parameter) => true;

        public void Execute(object parameter) => action();
    }
}