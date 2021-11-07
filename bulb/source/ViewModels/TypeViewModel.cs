using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows;

namespace Bulb {
    /// <summary>
    /// Viewmodel for each editor type.
    /// </summary>
    public class TypeViewModel : ViewModel {
        public string Name { get; }

        public string Value {
            get => value;
            set {
                this.value = value;
                OnPropertyChanged(nameof(Value));
            }
        }
        private string value;
        public Visibility ValueVisibility { get; }

        public ObservableCollection<TypeViewModel> Members { get; }
        public Visibility MembersVisibility { get; }

        public TypeViewModel(string name, List<TypeViewModel> members) {
            Name = name;

            Members = new ObservableCollection<TypeViewModel>(members);
            MembersVisibility = Members.Count > 0 ? Visibility.Visible : Visibility.Collapsed;

            ValueVisibility = Members.Count > 0 ? Visibility.Collapsed : Visibility.Visible;
        }
    }
}