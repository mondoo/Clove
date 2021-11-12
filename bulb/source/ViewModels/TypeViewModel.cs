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

                if (value.Length != 0) {
                    OnValueChanged?.Invoke(valueOffset, valueSize, this.value);
                }
            }
        }
        private string value;
        public Visibility ValueVisibility { get; }

        public ObservableCollection<TypeViewModel> Members { get; }
        public Visibility MembersVisibility { get; }

        public delegate void ValueChangedHandler(int valueOffset, int valueSize, string value);
        public ValueChangedHandler OnValueChanged;

        private int valueOffset;
        private int valueSize;

        public TypeViewModel(string name, List<TypeViewModel> members) {
            Name = name;
            Members = new ObservableCollection<TypeViewModel>(members);

            MembersVisibility = Visibility.Visible;
            ValueVisibility = Visibility.Collapsed;
        }

        public TypeViewModel(string name, int valueOffset, int valueSize, string value) {
            Name = name;
            this.valueOffset = valueOffset;
            this.valueSize = valueSize;
            this.value = value; //Bypass property to prevent delegate being fired.

            MembersVisibility = Visibility.Collapsed;
            ValueVisibility = Visibility.Visible;
        }
    }
}