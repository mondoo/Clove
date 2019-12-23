using Bulb.Core;
using Bulb.CoreUI;
using Bulb.Windows;
using System.Collections.ObjectModel;
using System.Windows.Input;

namespace Bulb.UI {
	public class EntityViewModel : ViewModel {
		public string Name { get; }
		public ObservableCollection<ComponentViewModel> Components { get; }
		public ICommand OnClickedCommand { get; }

		public delegate void OnClickedDelegate(EntityViewModel entity);
		public event OnClickedDelegate clickedEvent;

		public EntityViewModel(Entity entity) {
			Name = entity.name;

			var Comps = entity.getComponents();

			Components = new ObservableCollection<ComponentViewModel>();
			foreach(var comp in Comps) {
				Components.Add(new ComponentViewModel(comp));
			}

			OnClickedCommand = new RelayCommand(OnClicked);
		}

		private void OnClicked() {
			clickedEvent(this);
		}
	}
}