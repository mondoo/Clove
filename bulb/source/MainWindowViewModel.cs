using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Input;

namespace Garlic.Bulb
{
    public class MainWindowViewModel : ViewModel
    {
        //Commands
        public ICommand CreateEntityCommand { get; private set; }

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
            }
        }
        private EntityViewModel selectedEntity;

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
        }
    };
}