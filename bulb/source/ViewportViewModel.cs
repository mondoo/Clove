using System.Windows.Input;
using Membrane = garlic.membrane;

namespace Garlic.Bulb {
	/// <summary>
	/// Handles viewport input and correctly dispatches it to the engine.
	/// </summary>
	public class ViewportViewModel : ViewModel {
		public ICommand SelectedCommand { get; }

		public ViewportViewModel() {
			SelectedCommand = new RelayCommand(() => Membrane.Log.write(Membrane.LogLevel.Debug, "Kloop!"));
		}
	}
}