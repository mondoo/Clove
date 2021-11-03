using System.Windows.Input;
using System.Collections.ObjectModel;
using System.Collections.Generic;

namespace Bulb {
    /// <summary>
    /// Viewmodel for each editor visible member within a component.
    /// </summary>
    public class ComponentMemberViewModel : ViewModel {
        public string Name { get; }

        public ComponentMemberViewModel(string name) {
            Name = name;
        }
    }

    /// <summary>
    /// Viewmodel for displaying components in editor
    /// </summary>
    public class ComponentViewModel : ViewModel {
        public string Name { get; }

        public ObservableCollection<ComponentMemberViewModel> Members { get; }

        public ICommand RemoveComponentCommand { get; }

        public delegate void RemoveComponentHandler(/*Membrane.ComponentType Type*/);
        public RemoveComponentHandler OnRemoved;

        public ComponentViewModel(string name, List<ComponentMemberViewModel> members) {
            Name = name;
            Members = new ObservableCollection<ComponentMemberViewModel>(members);

            RemoveComponentCommand = new RelayCommand(() => { }); //TODO
        }
    }
}