using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Input;

namespace Garlic.Bulb
{
    public class MainWindowViewModel : ViewModel
    {
        //Commands
        public ICommand AddEntityCommand { get; private set; }

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

        public ObservableCollection<object> Entities { get; set; } = new ObservableCollection<object>();

        public delegate void AddEntityEventHandler();
        public event AddEntityEventHandler AddEntity;

        public MainWindowViewModel()
        {
            AddEntityCommand = new RelayCommand(() =>
            {
                if (AddEntity != null)
                {
                    AddEntity.Invoke();
                    Entities.Add(new object());
                }
            });
        }
    };
}