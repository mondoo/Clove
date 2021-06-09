using System.IO;

namespace Garlic.Bulb {
    /// <summary>
    /// Manages the current position in the file explorer.
    /// </summary>
    public class FileExplorerViewModel : ViewModel {
        /// <summary>
        /// The root directory this explorer views from.
        /// </summary>
        public DirectoryViewModel RootDirectory {
            get { return rootDirectory; }
            private set {
                rootDirectory = value;
                OnPropertyChanged(nameof(RootDirectory));
            }
        }
        private DirectoryViewModel rootDirectory;

        /// <summary>
        /// The current directory this explorer has opened into.
        /// </summary>
        public DirectoryViewModel CurrentDirectory {
            get { return currentDirectory; }
            private set {
                currentDirectory = value;
                OnPropertyChanged(nameof(currentDirectory));
            }
        }
        private DirectoryViewModel currentDirectory;

        public FileExplorerViewModel(string rootPath) {
            var directory = new DirectoryItemViewModel(Directory.CreateDirectory(rootPath));

            RootDirectory = new DirectoryViewModel(directory);
            CurrentDirectory = new DirectoryViewModel(directory);

            RootDirectory.OnItemOpened += OnItemOpened;
            CurrentDirectory.OnItemOpened += OnItemOpened;
        }

        private void OnItemOpened(DirectoryItemViewModel item) {
            //TODO: Support opening files
            if(item.Type == ObjectType.Directory) {
                CurrentDirectory = new DirectoryViewModel(item);
                CurrentDirectory.OnItemOpened += OnItemOpened;
            }
        }
    }
}