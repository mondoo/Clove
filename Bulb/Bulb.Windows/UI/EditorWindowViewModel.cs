using Bulb.Core;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;

namespace Bulb.UI {
	public class EditorWindowViewModel : ViewModel {
		public ICommand AddEntityCommand { get; set; }
		public ICommand AddComponentCommand { get; set; }

		public EntityViewModel SelectedEntity {
			get {
				return _SelectedEntity;
			}
			set {
				_SelectedEntity = value;
				OnPropertyChanged(this, new PropertyChangedEventArgs(nameof(SelectedEntity)));
			}
		}
		public ObservableCollection<EntityViewModel> Entities { get; set; }

		private EditorSession session;
		private EntityViewModel _SelectedEntity;

		public EditorWindowViewModel() {
			AddEntityCommand = new RelayCommand(AddEntity);
			AddComponentCommand = new RelayCommand(AddComponentToSelectedEntity);

			Entities = new ObservableCollection<EntityViewModel>();

			session = new EditorSession();
		}

		public void StartEngine(System.IntPtr hWnd, int posX, int posY, int width, int height) {
			session.Begin(hWnd, posX, posY, width, height);
		}

		public void StopEngine() {
			session.End();
		}

		public void RenderAreaSizeChanged(int x, int y, int width, int height) {
			session.UpdateWindowPosition(x, y);
			session.UpdateWindowSize(width, height);
		}

		private void AddEntity() {
			session.AddEntityToLayer();
			Entities.Add(new EntityViewModel());

			SelectedEntity = Entities[0];
		}

		private void AddComponentToSelectedEntity() {
			Console.WriteLine("Add component clicked");
		}
	}
}