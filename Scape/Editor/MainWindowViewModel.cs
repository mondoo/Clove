using System.ComponentModel;

namespace Editor
{
    public class MainWindowViewModel : ViewModel
    {
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
    };
}