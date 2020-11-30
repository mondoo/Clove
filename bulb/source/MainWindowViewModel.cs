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

        public ObservableCollection<EntityViewModel> Entities { get; set; } = new ObservableCollection<EntityViewModel>();

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

                    Entities.Add(entityVM);
                }
            });
        }
    };
}