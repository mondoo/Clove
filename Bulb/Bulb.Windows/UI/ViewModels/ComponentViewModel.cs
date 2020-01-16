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

		public float rotX {
			get {
				return component.getRotation().x;
			}
			set {
				var rotation = component.getRotation();
				rotation.x = value;
				component.setRotation(rotation);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(rotX)));
			}
		}
		public float rotY {
			get {
				return component.getRotation().y;
			}
			set {
				var rotation = component.getRotation();
				rotation.y = value;
				component.setRotation(rotation);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(rotY)));
			}
		}
		public float rotZ {
			get {
				return component.getRotation().z;
			}
			set {
				var rotation = component.getRotation();
				rotation.z = value;
				component.setRotation(rotation);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(rotZ)));
			}
		}

		public float scaleX {
			get {
				return component.getScale().x;
			}
			set {
				var scale = component.getScale();
				scale.x = value;
				component.setScale(scale);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(scaleX)));
			}
		}
		public float scaleY {
			get {
				return component.getScale().y;
			}
			set {
				var scale = component.getScale();
				scale.y = value;
				component.setScale(scale);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(scaleY)));
			}
		}
		public float scaleZ {
			get {
				return component.getScale().z;
			}
			set {
				var scale = component.getScale();
				scale.z = value;
				component.setScale(scale);
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(scaleZ)));
			}
		}

		private TransformComponent component;

		public TransformComponentViewModel(TransformComponent component) : base(component){
			//TODO: Should bind to events on the component and cache values instead of caching the component
			this.component = component;
		}
	}
}