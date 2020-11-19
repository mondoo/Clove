using System;
using System.ComponentModel;
using System.Threading;
using System.Windows;

namespace Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Garlic.Application app;

        private Thread appThread;
        private object appThreadLock = new object();

        private bool exit = false;

        public MainWindow()
        {
            InitializeComponent();

            app = new Garlic.Application();

            Closing += (object sender, CancelEventArgs e) =>
            {
                lock (appThreadLock)
                {
                    exit = true;
                }
                appThread.Join();
            };

            appThread = new Thread(new ThreadStart(TickApplication));
            appThread.Name = "Garlic application thread";
            appThread.Start();
        }

        private void TickApplication()
        {
            while (app.tick())
            {
                lock (appThreadLock)
                {
                    if (exit)
                    {
                        app.shutdown();
                    }
                }
            }
        }
    }
}
