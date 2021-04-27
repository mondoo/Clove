using System;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Threading;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.ComponentModel;

using Membrane = garlic.membrane;

namespace Garlic.Bulb {
    /// <summary>
    /// Interaction logic for Editor.xaml
    /// </summary>
    public partial class EditorApp : Application {
        private MainWindow editorWindow;
        private EditorSessionViewModel sessionViewModel;

        private Membrane.Application engineApp;

        //TODO: Move to MainWindow
        private WriteableBitmap imageSource;
        private IntPtr backBuffer;

        private Thread engineThread;
        private bool exitThread = false;

        private Size size = new Size(1, 1);
        private bool sizeChanged = false;
        private object resizeLock = new object();

        private void EditorStartup(object sender, StartupEventArgs e) {
            sessionViewModel = new EditorSessionViewModel();

            Membrane.Log.addSink((string message) => sessionViewModel.Log.LogText += message, "%v");

            //Initialise the editor window
            editorWindow = new MainWindow();

            editorWindow.DataContext = sessionViewModel;
            editorWindow.RenderArea.SizeChanged += OnRenderAreaSizeChanged;
            editorWindow.Closing += StopEngine;

            editorWindow.Show();
            MainWindow = editorWindow;

            CreateImageSource(size);

            //Initialise the engine
            engineApp = new Membrane.Application((int)size.Width, (int)size.Height);

            engineThread = new Thread(new ThreadStart(RunEngineApplication));
            engineThread.Name = "Garlic application thread";
            engineThread.Start();
        }

        private void OnRenderAreaSizeChanged(object sender, SizeChangedEventArgs e) {
            lock (resizeLock) {
                size = e.NewSize;
                CreateImageSource(size);
                sizeChanged = true;
            }
        }

        private void StopEngine(object sender, CancelEventArgs e) {
            exitThread = true;
            engineThread.Join();
        }

        private void CreateImageSource(Size size) {
            imageSource = new WriteableBitmap((int)size.Width, (int)size.Height, 96, 96, PixelFormats.Pbgra32, null);
            backBuffer = imageSource.BackBuffer;
            editorWindow.RenderArea.Source = imageSource;
        }

        private void RunEngineApplication() {
            while (!exitThread) {
                if (engineApp.isRunning()) {
                    //Send any editor events to the engine
                    Membrane.MessageHandler.flushEditor();

                    //Update the application
                    engineApp.tick();

                    //Render to image
                    lock (resizeLock) {
                        if (sizeChanged) {
                            engineApp.resize((int)size.Width, (int)size.Height);
                            sizeChanged = false;
                        }
                        engineApp.render(backBuffer);
                    }

                    //Update the image source through the dispatcher on the thread that owns the image
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Render, (Action)(() => {
                        imageSource.Lock();
                        imageSource.AddDirtyRect(new Int32Rect(0, 0, (int)imageSource.Width, (int)imageSource.Height));
                        imageSource.Unlock();
                    }));

                    //Send any engine events to the editor
                    Membrane.MessageHandler.flushEngine(Application.Current.Dispatcher);
                } else {
                    //Return to avoid calling shutdown if the app exits by itself
                    return;
                }
            }

            engineApp.shutdown();
        }
    }
}
