using System.Windows;

using Membrane = garlic.membrane;

namespace Garlic.Bulb {
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window {
		public MainWindow() {
			InitializeComponent();
		}

		private void EditorViewport_GotFocus(object sender, RoutedEventArgs e) {
			Membrane.Log.write(Membrane.LogLevel.Debug, "Render area focus recieved");
		}

		private void EditorViewport_KeyDown(object sender, System.Windows.Input.KeyEventArgs e) {
			Membrane.Log.write(Membrane.LogLevel.Debug, $"Key {e.Key} down on render area called.");
		}

		private void EditorViewport_KeyUp(object sender, System.Windows.Input.KeyEventArgs e) {
			Membrane.Log.write(Membrane.LogLevel.Debug, $"Key {e.Key} up on render area called.");
		}

		private void EditorViewport_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e) {
			Membrane.Log.write(Membrane.LogLevel.Debug, "Klops!");
		}

		private void EditorViewport_MouseEnter(object sender, System.Windows.Input.MouseEventArgs e) {
			//Membrane.Log.write(Membrane.LogLevel.Debug, "Render area mouse enter");
		}

		private void EditorViewport_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e) {
			//Membrane.Log.write(Membrane.LogLevel.Debug, "Render area mouse leave");
		}
	}
}
