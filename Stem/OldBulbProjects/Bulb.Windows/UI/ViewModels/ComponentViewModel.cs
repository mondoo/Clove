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
		public float PosX { 
			get {
				return position.x;
			}
			set {
				position.x = value;
				UpdateComponentPosition(position);
			}
		}
		public float PosY {
			get {
				return position.y;
			}
			set {
				position.y = value;
				UpdateComponentPosition(position);
			}
		}
		public float PosZ {
			get {
				return position.z;
			}
			set {
				position.z = value;
				UpdateComponentPosition(position);
			}
		}

		public float RotX {
			get {
				return rotation.x;
			}
			set {
				rotation.x = value;
				UpdateComponentRotation(rotation);
			}
		}
		public float RotY {
			get {
				return rotation.y;
			}
			set {
				rotation.y = value;
				UpdateComponentRotation(rotation);
			}
		}
		public float RotZ {
			get {
				return rotation.z;
			}
			set {
				rotation.z = value;
				UpdateComponentRotation(rotation);
			}
		}

		public float ScaleX {
			get {
				return scale.x;
			}
			set {
				scale.x = value;
				UpdateComponentScale(scale);
			}
		}
		public float ScaleY {
			get {
				return scale.y;
			}
			set {
				scale.y = value;
				UpdateComponentScale(scale);
			}
		}
		public float ScaleZ {
			get {
				return scale.z;
			}
			set {
				scale.z = value;
				UpdateComponentScale(scale);
			}
		}

		private vec3 position;
		private vec3 rotation;
		private vec3 scale;

		private event VecDelegate UpdateComponentPosition;
		private event VecDelegate UpdateComponentRotation;
		private event VecDelegate UpdateComponentScale;

		public TransformComponentViewModel(TransformComponent component) : base(component){
			component.onTransformChanged += new TransformDelegate(OnComponentTransformUpdated);

			position = component.getPosition();
			rotation = component.getRotation();
			scale = component.getScale();

			UpdateComponentPosition += new VecDelegate(component.setPosition);
			UpdateComponentRotation += new VecDelegate(component.setRotation);
			UpdateComponentScale += new VecDelegate(component.setScale);
		}

		private void OnComponentTransformUpdated(Transform componentTransform) {
			position = componentTransform.position;
			rotation = componentTransform.rotation;
			scale = componentTransform.scale;

			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(PosX)));
			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(PosY)));
			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(PosZ)));

			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(RotX)));
			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(RotY)));
			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(RotZ)));

			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(ScaleX)));
			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(ScaleY)));
			OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(ScaleZ)));
		}
	}
}