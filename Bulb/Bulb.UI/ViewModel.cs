using System.ComponentModel;

namespace Bulb.UI {
	public class ViewModel : INotifyPropertyChanged {
		public event PropertyChangedEventHandler PropertyChanged = (sender, e) => { };
	}
}