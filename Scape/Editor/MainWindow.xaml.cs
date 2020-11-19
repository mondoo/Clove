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

            appThread = new Thread(new ThreadStart(RunApplication));
            appThread.Name = "Garlic application thread";
            appThread.Start();

            //Make sure we notify the thread when we want to close
            Closing += (object sender, CancelEventArgs e) =>
            {
                lock (appThreadLock)
                {
                    exit = true;
                }
                appThread.Join();
            };
        }

        private void RunApplication()
        {
            while (app.isRunning())
            {
                bool shouldExit = false;
                lock (appThreadLock)
                {
                    shouldExit = exit;
                }

                if (shouldExit)
                {
                    app.shutdown();
                }
                else
                {
                    app.tick();
                }
            }
        }
    }
}
