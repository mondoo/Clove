using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Bulb {
    /// <summary>
    /// Viewmodel for each editor type.
    /// </summary>
    public class TypeViewModel : ViewModel {
        public string Name { get; }
        public ObservableCollection<TypeViewModel> Members { get; }

        public TypeViewModel(string name, List<TypeViewModel> members) {
            Name = name;
            Members = new ObservableCollection<TypeViewModel>(members);
        }
    }
}