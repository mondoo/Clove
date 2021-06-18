using System.IO;

namespace Garlic.Bulb {
    /// <summary>
    /// Manages the current position in the file explorer.
    /// </summary>
    public class FileExplorerViewModel : ViewModel {
        /// <summary>
        /// The root directory this explorer views from.
        /// </summary>
        public DirectoryItemViewModel RootDirectory {
            get { return rootDirectory; }
            private set {
                rootDirectory = value;
                OnPropertyChanged(nameof(RootDirectory));
            }
        }
        private DirectoryItemViewModel rootDirectory;

        /// <summary>
        /// The current directory this explorer has opened into.
        /// </summary>
        public DirectoryItemViewModel CurrentDirectory {
            get { return currentDirectory; }
            private set {
                currentDirectory = value;
                OnPropertyChanged(nameof(currentDirectory));
            }
        }
        private DirectoryItemViewModel currentDirectory;

        public FileExplorerViewModel(string rootPath) {
            RootDirectory = new DirectoryItemViewModel(rootPath);
            CurrentDirectory = new DirectoryItemViewModel(rootPath);

            //RootDirectory.OnOpened += OnItemOpened;
            //CurrentDirectory.OnOpened += OnItemOpened;
        }

        //private void OnItemOpened(DirectoryItemViewModel item) {
        //    //TODO: Support opening files
        //    if (item.Type == ObjectType.Directory) {
        //        CurrentDirectory = new DirectoryViewModel(item);
        //        CurrentDirectory.OnItemOpened += OnItemOpened;
        //    }
        //}
    }
}