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

			session.Begin(
				hWnd,
				(int)RenderArea.Margin.Left + 1, (int)RenderArea.Margin.Top + 1,
				(int)RenderArea.ActualWidth - 2, (int)RenderArea.ActualHeight - 2
			);
		}

		private void StopEngine() {
			session.End();
		}
    }
}
