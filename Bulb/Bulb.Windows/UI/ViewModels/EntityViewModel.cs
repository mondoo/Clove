using Bulb.Core;
using Bulb.CoreUI;
using Bulb.Windows;
using System.Collections.ObjectModel;
using System.Windows.Input;

namespace Bulb.UI {
	public class EntityViewModel : ViewModel {
		public string Name { get; }
		public ObservableCollection<ComponentViewModel> Components { get; set; }
		public ICommand OnClickedCommand { get; }

		public delegate void OnClickedDelegate(EntityViewModel entity);
		public event OnClickedDelegate clickedEvent;

		//TODO: Remove references to entity
		private readonly Entity entity;

		public EntityViewModel(Entity entity) {
			this.entity = entity;
			Name = entity.name;

			RefreshComponents();

			OnClickedCommand = new RelayCommand(OnClicked);
		}

		public void AddTransformComponent() {
			entity.addTransformComponent();
			RefreshComponents();
		}

		public void AddMeshComponent() {
			entity.addMeshComponent();
			RefreshComponents();
		}

		private void RefreshComponents() {
			var Comps = entity.getComponents();
			if(Comps != null) {
				Components = new ObservableCollection<ComponentViewModel>();
				foreach(var comp in Comps) {
					Components.Add(new ComponentViewModel(comp));
				}
			}
		}

		private void OnClicked() {
			clickedEvent(this);
		}
	}
}