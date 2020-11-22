using System;
using System.ComponentModel;
using System.Threading;
using System.Windows;
using System.Windows.Threading;
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
        private WriteableBitmap imageSource; //Owning this here for now as the UI thread needs to lock it

        private Thread appThread;
        private object appLock = new object();

        private bool exit = false;
        private object exitLock = new object();

        public MainWindow()
        {
            InitializeComponent();

            //Initialise and start the application loop
            int width = RenderArea.ActualWidth > 0 ? (int)RenderArea.ActualWidth : 1;
            int height = RenderArea.ActualHeight > 0 ? (int)RenderArea.ActualHeight : 1;

            CreateImageSource(new Size(width, width));

            app = new Garlic.Application(width, height);

            appThread = new Thread(new ThreadStart(RunApplication));
            appThread.Name = "Garlic application thread";
            appThread.Start();

            //Notify the app when ever we change size
            RenderArea.SizeChanged += OnRenderAreaSizeChanged;

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

        private void OnRenderAreaSizeChanged(object sender, SizeChangedEventArgs e)
        {
            CreateImageSource(e.NewSize);
            app.resize((int)e.NewSize.Width, (int)e.NewSize.Height);
        }

        private void CreateImageSource(Size size)
        {
            imageSource = new WriteableBitmap((int)size.Width, (int)size.Height, 96, 96, PixelFormats.Pbgra32, null);
            RenderArea.Source = imageSource;
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
                        //Update the application
                        app.tick();

                        //Tell the dispatcher on the thread that owns the image source to lock it
                        IntPtr backBuffer = new IntPtr();
                        Application.Current.Dispatcher.Invoke(DispatcherPriority.Render, (Action)(() =>
                        {
                            imageSource.Lock();
                            backBuffer = imageSource.BackBuffer;
                        }));

                        //Render to image
                        app.render(backBuffer);

                        //Tell the dispatcher on the thread that owns the image source to unlock it
                        Application.Current.Dispatcher.Invoke(DispatcherPriority.Render, (Action)(() =>
                        {
                            imageSource.AddDirtyRect(new Int32Rect(0, 0, (int)imageSource.Width, (int)imageSource.Height));
                            imageSource.Unlock();
                        }));
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
