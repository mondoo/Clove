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
		public float posX { 
			get {
				return component.getPosition().x;
			}
			set {
				var position = component.getPosition();
				position.x = value;
				component.setPosition(position);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(posX)));
			}
		}
		public float posY {
			get {
				return component.getPosition().y;
			}
			set {
				var position = component.getPosition();
				position.y = value;
				component.setPosition(position);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(posY)));
			}
		}
		public float posZ {
			get {
				return component.getPosition().z;
			}
			set {
				var position = component.getPosition();
				position.z = value;
				component.setPosition(position);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(posZ)));
			}
		}

		private TransformComponent component;

		public TransformComponentViewModel(TransformComponent component) : base(component){
			this.component = component;
		}
	}
}