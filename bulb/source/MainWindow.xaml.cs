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

		private void UserControl_KeyUp(object sender, System.Windows.Input.KeyEventArgs e) {
			Membrane.Log.write(Membrane.LogLevel.Debug, $"Key {e.Key} up on render area called.");
		}

		private void UserControl_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e) {
			Membrane.Log.write(Membrane.LogLevel.Debug, "Render area clicked");
		}

		private void UserControl_GotFocus(object sender, RoutedEventArgs e) {
			Membrane.Log.write(Membrane.LogLevel.Debug, "Render area focus recieved");
		}
	}
}
