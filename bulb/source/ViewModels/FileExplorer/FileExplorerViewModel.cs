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
            var directoryViewModel = new DirectoryItemViewModel(rootPath);
            directoryViewModel.OnOpened += OnItemOpened;

            rootDirectory = directoryViewModel;
            CurrentDirectory = directoryViewModel;
        }

        private void OnItemOpened(DirectoryItemViewModel item) {
            if (item.Type == ObjectType.Directory) {
                if(CurrentDirectory != RootDirectory) {
                    CurrentDirectory.OnOpened -= OnItemOpened;
                }

                CurrentDirectory = item;
                CurrentDirectory.OnOpened += OnItemOpened;
            }

            //TODO: Support opening files
        }
    }
}