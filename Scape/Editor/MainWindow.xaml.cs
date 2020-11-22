using System;
using System.ComponentModel;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Garlic.Application app;

        private Thread appThread;
        private object appLock = new object();

        private bool exit = false;
        private object exitLock = new object();

        public MainWindow()
        {
            InitializeComponent();

            //Initialise and start the application loop
            int width = (int)RenderArea.ActualWidth;
            int height = (int)RenderArea.ActualHeight;

            app = new Garlic.Application(width > 0 ? width : 1, height > 0 ? height : 1);

            appThread = new Thread(new ThreadStart(RunApplication));
            appThread.Name = "Garlic application thread";
            appThread.Start();

            //Notify the app when ever we change size
            RenderArea.SizeChanged += (object sender, SizeChangedEventArgs e) =>
            {
                lock(appLock)
                {
                    app.resize((int)RenderArea.ActualWidth, (int)RenderArea.ActualHeight);
                }
            };

            //Make sure we notify the thread when we want to close
            Closing += (object sender, CancelEventArgs e) =>
            {
                lock (exitLock)
                {
                    exit = true;
                }
                appThread.Join();
            };
        }

        private void RunApplication()
        {
            while (true)
            {
                lock (exitLock)
                {
                    if (exit)
                    {
                        lock (appLock)
                        {
                            app.shutdown();
                        }
                        break;
                    }
                }

                lock (appLock)
                {
                    if (app.isRunning())
                    {
                        app.tick();
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}
