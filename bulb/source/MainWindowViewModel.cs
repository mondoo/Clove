using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Input;
using System.Windows;

namespace Garlic.Bulb
{
    public class MainWindowViewModel : ViewModel
    {
        //Commands
        public ICommand CreateEntityCommand { get; private set; }
        public ICommand AddComponentCommand { get; private set; }

        //Properties
        public string LogText
        {
            get { return logText; }
            set
            {
                logText = value;
                OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(LogText)));
            }
        }
        private string logText;

        public ObservableCollection<EntityViewModel> Entities { get; } = new ObservableCollection<EntityViewModel>();

        public EntityViewModel SelectedEntity
        {
            get { return selectedEntity; }
            set
            {
                selectedEntity = value;
                OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(SelectedEntity)));
                OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(EntityViewVisibility)));
            }
        }
        private EntityViewModel selectedEntity;

        public Visibility EntityViewVisibility
        {
            get
            {
                return selectedEntity != null ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        public delegate void AddEntityEventHandler();
        public AddEntityEventHandler CreateEntity;

        public MainWindowViewModel()
        {
            CreateEntityCommand = new RelayCommand(() =>
            {
                if (CreateEntity != null)
                {
                    CreateEntity.Invoke();

                    var entityVM = new EntityViewModel();
                    entityVM.OnSelected += (EntityViewModel viewModel) => SelectedEntity = viewModel;

                    Entities.Add(entityVM);
                }
            });

            AddComponentCommand = new RelayCommand(() =>
            {
                if (SelectedEntity != null)
                {
                    SelectedEntity.Components.Add(new object());
                }
            });
        }
    };
}