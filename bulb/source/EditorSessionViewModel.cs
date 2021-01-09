using System;

namespace Garlic.Bulb {
	/// <summary>
	/// The View Model for the entire editor's session. Used to manage the viewmodels that make up the editor itself.
	/// </summary>
	public class EditorSessionViewModel : ViewModel {
		public SceneViewModel Scene {
			get { return scene; }
			set {
				scene = value;
				OnPropertyChanged(nameof(Scene));
			}
		}
		private SceneViewModel scene;

		public LogViewModel Log { get; } = new LogViewModel();

		public EditorSessionViewModel() {
			Scene = new SceneViewModel();
		}
	}
}