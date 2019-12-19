using Bulb.Core;
using System.Collections.ObjectModel;
using System.Windows.Input;

namespace Bulb.UI {
	public class EditorWindowViewModel : ViewModel {
		public ICommand AddEntityCommand { get; set; }

		//TODO: Put this into a seperate viewmodel?
		public ObservableCollection<EntityItemViewModel> Entities { get; set; }

		private EditorSession session;

		public EditorWindowViewModel() {
			AddEntityCommand = new RelayCommand(AddEntity);
			Entities = new ObservableCollection<EntityItemViewModel>();

			session = new EditorSession();
		}

		public void StartEngine(System.IntPtr hWnd, int posX, int posY, int width, int height) {
			session.Begin(hWnd, posX, posY, width, height);
		}

		public void StopEngine() {
			session.End();
		}

		private void AddEntity() {
			session.AddEntityToLayer();
			EntityItemViewModel entityVM = new EntityItemViewModel();
			entityVM.Name = "An Entity";
			Entities.Add(entityVM);
		}
	}
}