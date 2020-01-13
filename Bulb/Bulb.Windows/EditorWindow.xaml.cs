using System;
using System.Windows;
using System.Runtime.InteropServices;
using Bulb.Core;
using Bulb.CoreUI;
using Bulb.UI;
using System.Text.RegularExpressions;
using System.Windows.Input;

namespace Bulb.Windows {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class EditorWindow : Window
    {
		[DllImport("user32.dll", SetLastError = true)]
		static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

		public EditorWindow() {
			InitializeComponent();

			DataContext = new EditorWindowViewModel();

			Loaded += (sender, e) => StartEngine();
			Closing += (sender, e) => StopEngine();
		}

		private void StartEngine() {
			IntPtr hWnd = FindWindow(null, this.Title);

			Point renderAreaPoint = RenderArea.TransformToAncestor(Application.Current.MainWindow).Transform(new Point(0, 0));

			((EditorWindowViewModel)DataContext).StartEngine(
				hWnd,
				(int)renderAreaPoint.X + 2, (int)renderAreaPoint.Y + 1,
				(int)RenderArea.ActualWidth - 2, (int)RenderArea.ActualHeight - 2
			);
		}

		private void StopEngine() {
			((EditorWindowViewModel)DataContext).StopEngine();
		}

		//TODO: Change this to bind straight to the view model
		private void RenderArea_SizeChanged(object sender, SizeChangedEventArgs e) {
			Point renderAreaPoint = RenderArea.TransformToAncestor(Application.Current.MainWindow).Transform(new Point(0, 0));

			((EditorWindowViewModel)DataContext).RenderAreaSizeChanged(
				(int)renderAreaPoint.X + 2, (int)renderAreaPoint.Y + 1,
				(int)RenderArea.ActualWidth - 2, (int)RenderArea.ActualHeight - 2);
		}
	}
}
