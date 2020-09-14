using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using Scape.Membrane;

namespace Scape.Editor.Views {
	public class MainWindow : Window {
		public MainWindow() {
			InitializeComponent();
#if DEBUG
            this.AttachDevTools();
#endif

			//Create and run the engine from here
			Engine e = new Engine();
			e.start();
			e.run();
		}

		private void InitializeComponent() {
			AvaloniaXamlLoader.Load(this);
		}
	}
}
