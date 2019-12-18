using System;
using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.ComponentModel;
using Bulb.Core;

namespace Bulb.Windows {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
		#region VARIABLES

		EditorSession session;

		#endregion

		#region FUNCTIONS

		[DllImport("user32.dll", SetLastError = true)]
		static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

		public MainWindow() {
			InitializeComponent();

			session = new EditorSession();

			Loaded += (sender, e) => StartEngine();
			Closing += (sender, e) => StopEngine();
		}

        #endregion

		private void StartEngine() {
			IntPtr hWnd = FindWindow(null, this.Title);

			Point renderAreaPoint = RenderArea.TransformToAncestor(Application.Current.MainWindow).Transform(new Point(0, 0));

			session.Begin(
				hWnd,
				(int)renderAreaPoint.X + 2, (int)renderAreaPoint.Y + 1,
				(int)RenderArea.ActualWidth - 2, (int)RenderArea.ActualHeight - 2
			);
		}

		private void StopEngine() {
			session.End();
		}

		//TEMP: Doing code behind for now - my guess would be to make a viewmodel for the layer
		private void AddEntityButton_Click(object sender, RoutedEventArgs e) {
			session.AddEntityToLayer();
		}
	}
}
