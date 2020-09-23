using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using System;
using System.Runtime.InteropServices;

namespace Scape.Editor.Views {
	public class MainWindow : Window {
		[DllImport("F:/garlic/build/lib/Debug/Scape.Membrane.dll")]
		static extern void startEngine();

		[DllImport("F:/garlic/build/lib/Debug/Scape.Membrane.dll")]
		static extern void runEngine();

		public MainWindow() {
			InitializeComponent();
#if DEBUG
            this.AttachDevTools();
#endif

			Opened += (object sender, EventArgs e) => {
				startEngine();
				runEngine();
			};
		}

		private void InitializeComponent() {
			AvaloniaXamlLoader.Load(this);
		}
	}
}
