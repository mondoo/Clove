using System;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Threading;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.ComponentModel;

using Membrane = garlic.membrane;

namespace Garlic.Bulb
{
    /// <summary>
    /// Interaction logic for Editor.xaml
    /// </summary>
    public partial class EditorApp : Application
    {
        private MainWindow editorWindow;
        private EditorSessionViewModel sessionViewModel;

        private EditorLogger editorLogger;

        private Membrane.Application engineApp;

        //TODO: Move to MainWindow
        private WriteableBitmap imageSource;
        private IntPtr backBuffer;

        private Thread engineThread;
        private object updateEngineLock = new object();
        private bool exitThread = false;

        private Size size = new Size(1, 1);
        private bool sizeChanged = false;
        private object resizeLock = new object();

        private void EditorStartup(object sender, StartupEventArgs e)
        {
            //Initialise the editor window
            editorWindow = new MainWindow();
            sessionViewModel = new EditorSessionViewModel();

            sessionViewModel.OnCreateEntity = () =>
            {
                lock (updateEngineLock)
                {
                    return engineApp.addEntity();
                }
            };
            sessionViewModel.OnCreateComponent = (entityId, componentType) =>
            {
                lock (updateEngineLock)
                {
                    engineApp.createComponent(entityId, componentType);
                }
            };
            //TEMP
            sessionViewModel.OnSetPosition = (entityId, x, y, z) =>
            {
                lock (updateEngineLock)
                {
                    engineApp.setPosition(entityId, x, y, z);
                }
            };

            editorWindow.DataContext = sessionViewModel;
            editorWindow.RenderArea.SizeChanged += OnRenderAreaSizeChanged;
            editorWindow.Closing += StopEngine;

            //Forward the logs to the editor's window
            editorLogger = new EditorLogger();
            editorLogger.WriteTextEvent += (object sender2, TextEventArgs e2) => sessionViewModel.Log.LogText += e2.Text;
            Console.SetOut(editorLogger);

            editorWindow.Show();
            MainWindow = editorWindow;

            CreateImageSource(size);

            Membrane.MessageHandler.bindToMessage<Membrane::Engine_OnEntityCreated>(TestMessageHandling);

            //Initialise the engine
            engineApp = new Membrane.Application((int)size.Width, (int)size.Height);
            
            engineThread = new Thread(new ThreadStart(RunEngineApplication));
            engineThread.Name = "Garlic application thread";
            engineThread.Start();

        }

        private void TestMessageHandling(Membrane::Engine_OnEntityCreated entityCreatedMessage) {
            Console.WriteLine($"Message recieved! ({entityCreatedMessage.entity})");
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
                    lock (updateEngineLock)
                    {
                        engineApp.tick();
                    }

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
