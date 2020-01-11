using Bulb.Core;
using Bulb.CoreUI;
using System.ComponentModel;

namespace Bulb.UI {
	public class ComponentViewModel : ViewModel {
		public string Name { get; }

		public ComponentViewModel(Bulb.Core.Component component) {
			Name = component.name;
		}
	}

	public class TransformComponentViewModel : ComponentViewModel {
		public float X { 
			get {
				return component.getX();
			}
			set {
				component.setX(value);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(X)));
			}
		}
		public float Y {
			get {
				return component.getY();
			}
			set {
				component.setY(value);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(Y)));
			}
		}
		public float Z {
			get {
				return component.getZ();
			}
			set {
				component.setZ(value);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(Z)));
			}
		}

		private TransformComponent component;

		public TransformComponentViewModel(TransformComponent component) : base(component){
			this.component = component;
		}
	}
}