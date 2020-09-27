using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using System;
using System.Runtime.InteropServices;

namespace Scape.Editor.Views {
	public class MainWindow : Window {
		public MainWindow() {
			Scape.Engine.Application app = new Scape.Engine.Application();

			InitializeComponent();
#if DEBUG
            this.AttachDevTools();
#endif

			Opened += (object sender, EventArgs e) => {
				app.start();
				//app.run();
			};
		}

		private void InitializeComponent() {
			AvaloniaXamlLoader.Load(this);
		}
	}
}
