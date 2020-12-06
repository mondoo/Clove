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

        public ObservableCollection<ComponentViewModel> Components { get; } = new ObservableCollection<ComponentViewModel>();

        public ICommand SelectedCommand { get; }

        public delegate void SelectionHandler(EntityViewModel viewModel);
        public SelectionHandler OnSelected;

        public delegate ComponentViewModel AddComponentHandler();
        public AddComponentHandler OnAddComponent;

        public EntityViewModel()
        {
            SelectedCommand = new RelayCommand(() => OnSelected?.Invoke(this));
        }

        //TODO: Take in a component type
        public void AddComponent(){
            Components.Add(OnAddComponent?.Invoke());
        }
    }
}