using Bulb.Core;
using Bulb.CoreUI;

namespace Bulb.UI {
	public class ComponentViewModel : ViewModel {
		public string Name { get; }

		public ComponentViewModel(Bulb.Core.Component component) {
			Name = component.name;
		}
	}

	public class TransformComponentViewModel : ComponentViewModel {
		public float X { get; }
		public float Y { get; }
		public float Z { get; }

		public TransformComponentViewModel(Bulb.Core.TransformComponent component) : base(component){
			X = component.x;
			Y = component.y;
			Z = component.z;
		}
	}
}