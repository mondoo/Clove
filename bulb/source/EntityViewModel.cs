using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;

using Membrane = garlic.membrane;

namespace Garlic.Bulb {
	public class EntityViewModel : ViewModel {
		public uint EntityId { get; set; }

		public string Name {
			get { return name; }
			set {
				name = value;
				OnPropertyChanged(nameof(Name));

				var message = new Membrane.Editor_UpdateName();
				message.entity = EntityId;
				message.name = name;
				
				Membrane.MessageHandler.sendMessage(message);
			}
		}
		private string name = "New Entity";

		public ObservableCollection<ComponentViewModel> Components { get; } = new ObservableCollection<ComponentViewModel>();

		public ICommand SelectedCommand { get; }

		public delegate void SelectionHandler(EntityViewModel viewModel);
		public SelectionHandler OnSelected;

		public EntityViewModel() {
			//Bind to messages
			Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnComponentCreated>(OnComponentCreated);
			Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnTransformChanged>(OnTransformChanged);

			//Set up commands
			SelectedCommand = new RelayCommand(() => OnSelected?.Invoke(this));
		}

		public EntityViewModel(List<Membrane.ComponentType> components) : this() {
			foreach (var componentType in components) {
				Components.Add(CreateComponentViewModel(componentType));
			}
		}

		public void AddComponent(Membrane.ComponentType type) {
			var message = new Membrane.Editor_CreateComponent();
			message.entity = EntityId;
			message.componentType = type;

			Membrane.MessageHandler.sendMessage(message);
		}

		private void OnComponentCreated(Membrane.Engine_OnComponentCreated message) {
			if(EntityId == message.entity) {
				Components.Add(CreateComponentViewModel(message.componentType));
			}
		}

		private ComponentViewModel CreateComponentViewModel(Membrane.ComponentType type) {
			ComponentViewModel componentVm;

			switch(type) {
				case Membrane.ComponentType.Transform: {
					var transformComp = new TransformComponentViewModel();
					transformComp.OnTransformChanged = UpdateTransform;

					componentVm = transformComp;
				}
				break;
				default:
					componentVm = new ComponentViewModel();
					break;
			}

			componentVm.Name = $"{type}";

			return componentVm;
		}

		private void UpdateTransform(Membrane.Vector3 position, Membrane.Vector3 rotation, Membrane.Vector3 scale) {
			var message = new Membrane.Editor_UpdateTransform();
			message.entity = EntityId;
			message.position = position;
			message.rotation = rotation;
			message.scale = scale;

			Membrane.MessageHandler.sendMessage(message);
		}

		private void OnTransformChanged(Membrane.Engine_OnTransformChanged message) {
			if(EntityId == message.entity) {
				foreach(ComponentViewModel comp in Components) {
					if(comp.GetType() == typeof(TransformComponentViewModel)) {
						var transform = (TransformComponentViewModel)comp;
						transform.Update(message.position, message.rotation, message.scale);
						break;
					}
				}
			}
		}
	}
}