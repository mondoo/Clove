using System.Windows;
using System.Windows.Input;

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

		}

		private void EditorViewport_Key(object sender, KeyEventArgs e) {
			var message = new Membrane.Editor_ViewportKeyEvent();
			message.key = e.Key;
			message.type = e.IsDown ? Membrane.Editor_ViewportKeyEvent.Type.Pressed : Membrane.Editor_ViewportKeyEvent.Type.Released;

			Membrane.MessageHandler.sendMessage(message);
		}

		private void EditorViewport_MouseLeftButtonUp(object sender, MouseButtonEventArgs e) {
			if(EditorViewport.IsFocused) {
				//TODO: Mouse events
			} else {
				//Input propagation relies on the viewport being focused
				EditorViewport.Focus();
			}
		}

		private void EditorViewport_MouseEnter(object sender, MouseEventArgs e) {
			//Membrane.Log.write(Membrane.LogLevel.Debug, "Render area mouse enter");
		}

		private void EditorViewport_MouseLeave(object sender, MouseEventArgs e) {
			//Membrane.Log.write(Membrane.LogLevel.Debug, "Render area mouse leave");
		}
	}
}
