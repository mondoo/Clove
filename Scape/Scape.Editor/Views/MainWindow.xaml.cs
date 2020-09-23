using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using Scape.Membrane;
using System;

namespace Scape.Editor.Views {
	public class MainWindow : Window {
		public MainWindow() {
			InitializeComponent();
#if DEBUG
            this.AttachDevTools();
#endif

			Opened += (object sender, EventArgs e) => {
				Engine engine = new Engine();
				engine.start();
				engine.run();
			};
		}

		private void InitializeComponent() {
			AvaloniaXamlLoader.Load(this);
		}
	}
}
