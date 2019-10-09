using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System;
using CloveWrapper;
using System.Threading.Tasks;

namespace Editor {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private CancellationTokenSource cancelTokenSource;
        private CancellationToken token;

        private readonly Wrapper wp;

        public MainWindow() {
            InitializeComponent();

            cancelTokenSource = new CancellationTokenSource();
            token = cancelTokenSource.Token;

            wp = new Wrapper();
        }

        [DllImport("user32.dll", SetLastError = true)]
        static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        private void Button_Click(object sender, RoutedEventArgs e) {
            IntPtr hWnd = FindWindow(null, this.Title);
            //This is an extremely jank solution to check for the problem
            //Clove will either need to run on another thread 
            //or (more appropriately) not have the while loop in run
            Task testTask = new Task(async () => await (runClove(token, hWnd)));
            testTask.Start();

            Console.WriteLine("click");

            //Console.ReadLine();
        }

        private void Button2_Click(object sender, RoutedEventArgs e) {
            wp.StopClove();
        }

        private async Task runClove(CancellationToken token, IntPtr hWnd) {
            wp.OpenClove(hWnd);
        }
    }
}
