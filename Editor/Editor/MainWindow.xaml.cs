using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System;
//Reg C++/CLR (No works)
using CloveWrapper;

namespace Editor {
    //NOTE: this has to be on the x64 CPU (proj -> properties -> build)

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //Reg C++ (Works)
        //[DllImport("CloveWrapper.dll")]
        //private static extern void OpenClove();

        public MainWindow() {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e) {
            //Reg C++/CLR (No works)
            Wrapper wp = new Wrapper();
            wp.OpenClove();

            //Reg C++ (Works)
            //OpenClove();
        }
    }
}
