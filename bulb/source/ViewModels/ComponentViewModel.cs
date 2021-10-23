using System.Windows.Input;
using System.Collections.ObjectModel;

using Membrane = membrane;

namespace Bulb {
    /// <summary>
    /// Viewmodel for each editor visible member within a component.
    /// </summary>
    public class ComponentMemberViewModel : ViewModel {
        public string Name { get; }

    }

    /// <summary>
    /// Viewmodel for displaying components in editor
    /// </summary>
    public class ComponentViewModel : ViewModel {
        public string Name { get; }

        ObservableCollection<ComponentMemberViewModel> members = new ObservableCollection<ComponentMemberViewModel>();

        public ICommand RemoveComponentCommand { get; }

        public delegate void RemoveComponentHandler(/*Membrane.ComponentType Type*/);
        public RemoveComponentHandler OnRemoved;

        public ComponentViewModel(string name) {
            Name = name;

            RemoveComponentCommand = new RelayCommand(() => { }); //TODO
        }
    }
}