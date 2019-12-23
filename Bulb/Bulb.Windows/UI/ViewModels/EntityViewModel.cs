using Bulb.Core;
using Bulb.CoreUI;
using Bulb.Windows;
using System.Collections.ObjectModel;

namespace Bulb.UI {
	public class EntityViewModel : ViewModel {
		public string Name { get; }
		public ObservableCollection<ComponentViewModel> Components { get; }

		public EntityViewModel(Entity entity) {
			Name = entity.name;

			var Comps = entity.getComponents();

			Components = new ObservableCollection<ComponentViewModel>();
			foreach(var comp in Comps) {
				Components.Add(new ComponentViewModel(comp));
			}
		}
	}
}