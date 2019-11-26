using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System;
using System.Threading.Tasks;
using Bulb.Engine;

namespace Bulb.Desktop.Windows {
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

        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        static extern IntPtr CreateWindowEx(
            int dwExStyle,
            string lpszClassName,
            string lpszWindowName,
            int style,
            int x, int y,
            int width, int height,
            IntPtr hwndParent,
            IntPtr hMenu,
            IntPtr hInst,
            [MarshalAs(UnmanagedType.AsAny)] object pvParam
        );

        private void Button_Click(object sender, RoutedEventArgs e) {
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


            //This is an extremely jank solution to check for the problem
            //Clove will either need to run on another thread 
            //or (more appropriately) not have the while loop in run
            Task testTask = new Task(async () => await (RunClove(token, RenderWindow, (int)RenderArea.ActualWidth, (int)RenderArea.ActualHeight)));
            testTask.Start();
        }

        private void Button2_Click(object sender, RoutedEventArgs e) {
            wp.StopClove();
        }

        private async Task RunClove(CancellationToken token, IntPtr hWnd, int width, int height) {
            wp.OpenClove(hWnd, width, height);
        }
    }
}
