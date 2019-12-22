using Bulb.UI;
using Bulb.Windows;
using System.Collections.ObjectModel;

public class EntityViewModel : ViewModel {
	public string Name { get; set; } = "Test Entity Name";
	public ObservableCollection<ComponentViewModel> Components { get; set; }

	public EntityViewModel() {
		Components = new ObservableCollection<ComponentViewModel>();
		Components.Add(new ComponentViewModel());
	}
}