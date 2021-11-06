using System.Windows.Input;
using System.Collections.ObjectModel;
using System.Collections.Generic;

namespace Bulb {
    /// <summary>
    /// Viewmodel for displaying components in editor
    /// </summary>
    public class ComponentViewModel : ViewModel {
        public string Name { get; }

        public ObservableCollection<TypeViewModel> Members { get; }

        public ICommand RemoveComponentCommand { get; }

        public delegate void RemoveComponentHandler(/*Membrane.ComponentType Type*/);
        public RemoveComponentHandler OnRemoved;

        public ComponentViewModel(string name, List<TypeViewModel> members) {
            Name = name;
            Members = new ObservableCollection<TypeViewModel>(members);

            RemoveComponentCommand = new RelayCommand(() => { }); //TODO
        }
    }
}