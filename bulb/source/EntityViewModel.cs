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
                OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(Name)));
            }
        }
        private string name = "New Entity";

        public ICommand SelectedCommand { get; private set; }

        public delegate void SelectionHandler(EntityViewModel viewModel);
        public event SelectionHandler OnSelected;

        public EntityViewModel()
        {
            SelectedCommand = new RelayCommand(() => OnSelected?.Invoke(this));
        }
    }
}