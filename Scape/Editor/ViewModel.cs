using System.ComponentModel;

namespace Editor
{
    public class ViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged = (sender, e) => { };

        protected void OnPropertyChanged(object sender, PropertyChangedEventArgs e) => PropertyChanged(sender, e);
    }
}