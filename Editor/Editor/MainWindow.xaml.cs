using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System;
using CloveWrapper;

namespace Editor {
    //NOTE: this has to be on the x64 CPU (proj -> properties -> build)

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow() {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e) {
            Wrapper wp = new Wrapper();
            wp.OpenClove();
        }
    }
}
