using System.Threading;
using System.Windows;
using System.ComponentModel;

using Membrane = membrane;

namespace Bulb {
    /// <summary>
    /// Interaction logic for Editor.xaml
    /// </summary>
    public partial class EditorApp : Application {
        private MainWindow editorWindow;
        private ProjectSelector projectSelector;

        private EditorSessionViewModel sessionViewModel;

        private Membrane.Application engineApp;

        private Thread engineThread;
        private bool exitThread = false;

        private Size size = new Size(1, 1);

        private void EditorStartup(object sender, StartupEventArgs e) {
            //Initialise the engine
            engineApp = new Membrane.Application((int)size.Width, (int)size.Height);

            //Check if the engine can load a previously used project or if we need to create a new one
            if (!engineApp.hasDefaultProject()) {
                OpenProjectSelector();
            } else {
                engineApp.openDefaultProject();
                StartEditorSession();
            }
        }

        private void OpenProjectSelector() {
            //Change to explicit shut down to stop when project selector closes
            ShutdownMode = ShutdownMode.OnExplicitShutdown;

            projectSelector = new ProjectSelector();
            projectSelector.OnProjectSelected += OnProjectSelected;

            projectSelector.Show();
        }

        private void OnProjectSelected(ProjectSelectedArgs args) {
            if (args.IsNewProject) {
                ProjectCreator creator = new ProjectCreator();
                creator.ProjectName = args.ProjectName;
                creator.ProjectPath = args.ProjectPath;
                creator.Create();
            }

            engineApp.openProject(args.FullPath);

            projectSelector.Close();

            StartEditorSession();
        }

        private void StartEditorSession() {
            ShutdownMode = ShutdownMode.OnLastWindowClose;

            //Set up the engine session
            sessionViewModel = new EditorSessionViewModel(".");

            Membrane.Log.addSink((string message) => sessionViewModel.Log.LogText += message, "%v");

            //Initialise the editor window
            editorWindow = new MainWindow {
                DataContext = sessionViewModel
            };
            editorWindow.Closing += StopEngine;

            editorWindow.Show();
            MainWindow = editorWindow;

            //Run the engine thread
            engineThread = new Thread(new ThreadStart(RunEngineApplication)) {
                Name = "Garlic application thread"
            };
            engineThread.Start();
        }

        public string resolveVfsPath(string path) {
            //TODO: Currently no thread locking here. Will be fine in most situations as the mounted paths are unlikely to change once the app has initialised
            return engineApp.resolveVfsPath(path);
        }

        private void StopEngine(object sender, CancelEventArgs e) {
            exitThread = true;
            engineThread.Join();
        }

        private void RunEngineApplication() {
            while (!exitThread) {
                if (engineApp.isRunning()) {
                    //Send any editor events to the engine
                    Membrane.MessageHandler.flushEditor();

                    lock (editorWindow.EditorViewport.ResizeMutex) {
                        //Resize before rendering
                        if (size != editorWindow.EditorViewport.Size) {
                            size = editorWindow.EditorViewport.Size;

                            engineApp.resize((int)size.Width, (int)size.Height);
                        }

                        //Update the application
                        engineApp.tick();

                        //Render to image
                        editorWindow.EditorViewport.WriteToBackBuffer(engineApp.render);
                    }

                    //Send any engine events to the editor
                    Membrane.MessageHandler.flushEngine(Current.Dispatcher);
                } else {
                    //Return to avoid calling shutdown if the app exits by itself
                    return;
                }
            }

            engineApp.shutdown();
        }
    }
}
