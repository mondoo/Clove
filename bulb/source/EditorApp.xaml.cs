using System;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Threading;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace Garlic.Bulb
{
    /// <summary>
    /// Interaction logic for Editor.xaml
    /// </summary>
    public partial class EditorApp : Application
    {
        private MainWindow editorWindow;
        private MainWindowViewModel editorWindowViewModel;

        private EditorLogger editorLogger;

        private garlic.membrane.Application engineApp;

        private WriteableBitmap imageSource; //Owning this here for now as the UI thread needs to lock it
        private IntPtr backBuffer;

        private Thread engineThread;
        private object threadLock = new object();
        private bool exitThread = false;

        private Size size = new Size();
        private bool sizeChanged = false;
        private object resizeLock = new object();

        private void EditorStartup(object sender, StartupEventArgs e)
        {
            editorWindowViewModel = new MainWindowViewModel();
            editorWindowViewModel.CreateEntity = CreateEntity;

            editorLogger = new EditorLogger();
            editorLogger.WriteTextEvent += (object sender2, TextEventArgs e2) => editorWindowViewModel.LogText += e2.Text;

            Console.SetOut(editorLogger);

            //Initialise the editor window
            editorWindow = new MainWindow();
            editorWindow.DataContext = editorWindowViewModel;

            editorWindow.RenderArea.SizeChanged += OnRenderAreaSizeChanged;
            editorWindow.Closing += StopEngine;

            editorWindow.Show();
            MainWindow = editorWindow;

            //Initialise and start the application loop
            int width = editorWindow.RenderArea.ActualWidth > 0 ? (int)editorWindow.RenderArea.ActualWidth : 1;
            int height = editorWindow.RenderArea.ActualHeight > 0 ? (int)editorWindow.RenderArea.ActualHeight : 1;
            size = new Size(width, width);
            CreateImageSource(size);

            engineApp = new garlic.membrane.Application(width, height);

            engineThread = new Thread(new ThreadStart(RunEngineApplication));
            engineThread.Name = "Garlic application thread";
            engineThread.Start();
        }

        private void CreateEntity()
        {
            lock (threadLock)
            {
                engineApp.addEntity();
            }
        }

        private void OnRenderAreaSizeChanged(object sender, SizeChangedEventArgs e)
        {
            lock (resizeLock)
            {
                size = e.NewSize;
                CreateImageSource(size);
                sizeChanged = true;
            }
        }

        private void StopEngine(object sender, CancelEventArgs e)
        {
            exitThread = true;
            engineThread.Join();
        }

        private void CreateImageSource(Size size)
        {

            imageSource = new WriteableBitmap((int)size.Width, (int)size.Height, 96, 96, PixelFormats.Pbgra32, null);
            backBuffer = imageSource.BackBuffer;
            editorWindow.RenderArea.Source = imageSource;
        }

        private void RunEngineApplication()
        {
            while (!exitThread)
            {
                if (engineApp.isRunning())
                {
                    //Update the application
                    engineApp.tick();

                    //Render to image
                    lock (resizeLock)
                    {
                        if (sizeChanged)
                        {
                            engineApp.resize((int)size.Width, (int)size.Height);
                            sizeChanged = false;
                        }
                        engineApp.render(backBuffer);
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

            engineApp.shutdown();
        }
    }
}
