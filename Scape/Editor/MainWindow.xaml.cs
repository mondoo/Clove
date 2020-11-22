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
        private IntPtr backBuffer;
        private object backBufferLock = new object();

        private Thread appThread;
        private bool exitThread = false;

        private Size size = new Size();
        private bool sizeChanged = false;

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
                exitThread = true;
                appThread.Join();
            };
        }

        private void OnRenderAreaSizeChanged(object sender, SizeChangedEventArgs e)
        {
            CreateImageSource(e.NewSize);
            sizeChanged = true;
            size = e.NewSize;
        }

        private void CreateImageSource(Size size)
        {
            lock (backBufferLock)
            {
                imageSource = new WriteableBitmap((int)size.Width, (int)size.Height, 96, 96, PixelFormats.Pbgra32, null);
                backBuffer = imageSource.BackBuffer;
            }
            RenderArea.Source = imageSource;
        }

        private void RunApplication()
        {
            while (!exitThread)
            {
                if (sizeChanged)
                {
                    app.resize((int)size.Width, (int)size.Height);
                    sizeChanged = false;
                }

                if (app.isRunning())
                {
                    //Update the application
                    app.tick();

                    //Render to image
                    lock (backBufferLock)
                    {
                        app.render(backBuffer);
                    }

                    //Update the image source through the dispatcher on the thread that owns the image
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Render, (Action)(() =>
                    {
                        imageSource.Lock();
                        imageSource.AddDirtyRect(new Int32Rect(0, 0, (int)imageSource.Width, (int)imageSource.Height));
                        imageSource.Unlock();
                    }));
                }
                else
                {
                    //Return to avoid calling shutdown if the app exits by itself
                    return;
                }
            }

            app.shutdown();
        }
    }
}
