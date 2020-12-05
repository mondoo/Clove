using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;

namespace Garlic.Bulb
{
    public class EntityViewModel : ViewModel
    {
        public string Name
        {
            get { return name; }
            set
            {
                name = value;
                OnPropertyChanged(nameof(Name));
            }
        }
        private string name = "New Entity";

        public ObservableCollection<object> Components { get; set; } = new ObservableCollection<object>();

        public ICommand SelectedCommand { get; private set; }

        public delegate void SelectionHandler(EntityViewModel viewModel);
        public event SelectionHandler OnSelected;

        public EntityViewModel()
        {
            SelectedCommand = new RelayCommand(() => OnSelected?.Invoke(this));
        }
    }
}