using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System;
using CloveWrapper;

namespace Editor {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow() {
            InitializeComponent();
        }

        [DllImport("user32.dll", SetLastError = true)]
        static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        private void Button_Click(object sender, RoutedEventArgs e) {
            Wrapper wp = new Wrapper();
            //wp.OpenClove();

            IntPtr hWnd = FindWindow(null, this.Title);
            wp.OpenClove(hWnd);
        }
    }
}
