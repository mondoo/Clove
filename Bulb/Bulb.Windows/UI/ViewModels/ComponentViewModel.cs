using Bulb.Core;
using Bulb.CoreUI;

namespace Bulb.UI {
	public class ComponentViewModel : ViewModel {
		public string Name { get; }
		public ComponentType Type { get; }

		public ComponentViewModel(Bulb.Core.Component component) {
			Name = component.name;
			Type = component.getType();
		}
	}
}