using Bulb.CoreUI;

namespace Bulb.UI {
	public class ComponentViewModel : ViewModel {
		public string Name { get; }

		public ComponentViewModel(Bulb.Core.Component component) {
			Name = component.name;
		}
	}
}