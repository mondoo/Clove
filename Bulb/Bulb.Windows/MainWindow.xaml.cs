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

		[DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
		static extern IntPtr CreateWindowEx(int dwExStyle, string lpszClassName, string lpszWindowName, int style, int x, int y, int width, int height, IntPtr hwndParent, IntPtr hMenu, IntPtr hInst, [MarshalAs(UnmanagedType.AsAny)] object pvParam);

		public MainWindow() {
			InitializeComponent();

			Loaded += (sender, e) => StartEngine();
			Closing += (sender, e) => StopEngine();

			session = new EditorSession();
		}

        #endregion

		private void StartEngine() {
			IntPtr hWnd = FindWindow(null, this.Title);

			IntPtr RenderWindow = CreateWindowEx(0,
			   "static",
			   "",
			   0x40000000 | 0x10000000, //WS_CHILD | WS_VISIBLE
			   (int)RenderArea.Margin.Left + 1, (int)RenderArea.Margin.Top + 1,
			   (int)RenderArea.ActualWidth - 2, (int)RenderArea.ActualHeight - 2,
			   hWnd,
			   IntPtr.Zero,
			   IntPtr.Zero,
			   0);

			session.Begin(RenderWindow, (int)RenderArea.ActualWidth - 2, (int)RenderArea.ActualHeight - 2);
		}

		private void StopEngine() {
			session.End();
		}
    }
}
