using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows;

using Membrane = membrane;

namespace Bulb {
    /// <summary>
    /// Viewmodel for each editor visible type.
    /// </summary>
    public class TypeViewModel : ViewModel {
        public string Name { get; }

        public string Value {
            get => value;
            set {
                this.value = value;
                OnPropertyChanged(nameof(Value));

                if (value.Length != 0) {
                    OnValueChanged?.Invoke(typeName, this.value);
                }
            }
        }
        private string value;
        public Visibility ValueVisibility { get; }

        public ObservableCollection<TypeViewModel> Members { get; }
        public Visibility MembersVisibility { get; }

        public delegate void ValueChangedHandler(string name, string value);
        public ValueChangedHandler OnValueChanged;

        private readonly string typeName;

        public TypeViewModel(string displayName, string typeName, List<TypeViewModel> members) {
            Name = displayName;
            this.typeName = typeName;
            Members = new ObservableCollection<TypeViewModel>(members);

            MembersVisibility = Visibility.Visible;
            ValueVisibility = Visibility.Collapsed;
        }

        public TypeViewModel(string displayName, string typeName, string value) {
            Name = displayName;
            this.typeName = typeName;
            this.value = value; //Bypass property to prevent delegate being fired.

            MembersVisibility = Visibility.Collapsed;
            ValueVisibility = Visibility.Visible;
        }
    }
}