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
                    OnValueChanged?.Invoke(offsetIntoParent, this.value);
                }
            }
        }
        private string value;
        public Visibility ValueVisibility { get; }

        public ObservableCollection<TypeViewModel> Members { get; }
        public Visibility MembersVisibility { get; }

        public delegate void ValueChangedHandler(uint offset, string value);
        public ValueChangedHandler OnValueChanged;

        private readonly uint offsetIntoParent;

        public TypeViewModel(string displayName, List<TypeViewModel> members) {
            Name = displayName;
            Members = new ObservableCollection<TypeViewModel>(members);

            MembersVisibility = Visibility.Visible;
            ValueVisibility = Visibility.Collapsed;
        }

        public TypeViewModel(string displayName, uint offset, string value) {
            Name = displayName;
            offsetIntoParent = offset;
            this.value = value; //Bypass property to prevent delegate being fired.

            MembersVisibility = Visibility.Collapsed;
            ValueVisibility = Visibility.Visible;
        }
    }
}