using System;
using System.Windows.Input;

using Membrane = garlic.membrane;

namespace Garlic.Bulb {
	/// <summary>
	/// The View Model for the entire editor's session. Used to manage the viewmodels that make up the editor itself.
	/// </summary>
	public class EditorSessionViewModel : ViewModel {
		public ICommand LoadSceneCommand { get; }
		public ICommand SaveSceneCommand { get; }

		public SceneViewModel Scene {
			get { return scene; }
			set {
				scene = value;
				OnPropertyChanged(nameof(Scene));
			}
		}
		private SceneViewModel scene = new SceneViewModel();

		public LogViewModel Log { get; } = new LogViewModel();

		public EditorSessionViewModel() {
			LoadSceneCommand = new RelayCommand(() => { 
				Membrane.Log.write(Membrane.LogLevel.Debug, "Load command sent");
				Membrane.MessageHandler.sendMessage(new Membrane.Editor_LoadScene());
			});
			SaveSceneCommand = new RelayCommand(() => {
				Membrane.Log.write(Membrane.LogLevel.Debug, "Save command sent");
				Membrane.MessageHandler.sendMessage(new Membrane.Editor_SaveScene());
			});
		}
	}
}