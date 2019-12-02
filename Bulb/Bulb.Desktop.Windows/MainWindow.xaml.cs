using System;
using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using Bulb.Engine;
using System.ComponentModel;

namespace Bulb.Desktop.Windows {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
		#region VARIABLES

		#endregion

		#region FUNCTIONS

		[DllImport("user32.dll", SetLastError = true)]
		static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

		[DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
		static extern IntPtr CreateWindowEx(int dwExStyle, string lpszClassName, string lpszWindowName, int style, int x, int y, int width, int height, IntPtr hwndParent, IntPtr hMenu, IntPtr hInst, [MarshalAs(UnmanagedType.AsAny)] object pvParam);

		public MainWindow() {
			InitializeComponent();

			Loaded += (object sender, RoutedEventArgs e) => StartEngine();
			Closing += (object sender, CancelEventArgs e) => StopEngine();
		}

        #endregion

		private void StartEngine() {
			IntPtr hWnd = FindWindow(null, this.Title);

			IntPtr RenderWindow = CreateWindowEx(0,
			   "static",
			   "",
			   0x40000000 | 0x10000000, //WS_CHILD | WS_VISIBLE
			   (int)RenderArea.Margin.Left, (int)RenderArea.Margin.Top,
			   (int)RenderArea.ActualWidth, (int)RenderArea.ActualHeight,
			   hWnd,
			   IntPtr.Zero,
			   IntPtr.Zero,
			   0);

			EngineSession.begin(RenderWindow, (int)RenderArea.ActualWidth, (int)RenderArea.ActualHeight);
		}

		private void StopEngine() {
			EngineSession.end();
		}
    }
}
