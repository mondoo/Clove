using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Input;

namespace Garlic.Bulb {
    public enum ObjectType {
        Directory,
        File
    }

    /// <summary>
    /// Contains information for an entire single directory or file.
    /// </summary>
    public class DirectoryItemViewModel : ViewModel {
        /// <summary>
        /// Name of this item.
        /// </summary>
        public string Name {
            get => name;
            private set {
                name = value;
                OnPropertyChanged(nameof(Name));
            }
        }
        private string name;

        /// <summary>
        /// The Virtual File System path of this item.
        /// </summary>
        public string VfsPath { get; private set; }

        /// <summary>
        /// Full system path of this item.
        /// </summary>
        public string FullPath => ConvertVfsPathToSystemPath(VfsPath);

        public ObjectType Type { get; }

        /// <summary>
        /// Parent directory of this item.
        /// </summary>
        public DirectoryItemViewModel Parent { get; private set; }

        public ICommand OpenCommand { get; }
        public ICommand DeleteCommand { get; }

        public delegate void ItemEventHandler(DirectoryItemViewModel item);
        public ItemEventHandler OnOpened;
        public ItemEventHandler OnDeleted;

        /// <summary>
        /// A list of all directories within this directory. Will be empty if this item is a file.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> SubDirectories { get; } = new ObservableCollection<DirectoryItemViewModel>();

        /// <summary>
        /// A list of all files within this directory. Will be empty if this item is a file.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> Files { get; } = new ObservableCollection<DirectoryItemViewModel>();

        /// <summary>
        /// A list of every single item within this directory. Will be empty if this item is a file.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> AllItems { get; } = new ObservableCollection<DirectoryItemViewModel>();

        private readonly FileSystemWatcher watcher;

        public DirectoryItemViewModel(string vfsPath) : this(new DirectoryInfo(ConvertVfsPathToSystemPath(vfsPath))) { }

        public DirectoryItemViewModel(DirectoryInfo directory) : this() {
            Name = directory.Name;
            VfsPath = ConvertSystemPathToVfsPath(directory.FullName);

            Type = ObjectType.Directory;

            watcher = new FileSystemWatcher(FullPath, "*.*") {
                EnableRaisingEvents = true
            };
            watcher.Created += OnFileCreated;
            watcher.Deleted += OnFileDeleted;
            watcher.Renamed += OnFileRenamed;

            foreach (DirectoryInfo dir in directory.EnumerateDirectories()) {
                DirectoryItemViewModel vm = CreateItem(dir);

                SubDirectories.Add(vm);
                AllItems.Add(vm);
            }
            foreach (FileInfo file in directory.EnumerateFiles()) {
                DirectoryItemViewModel vm = CreateItem(file);

                Files.Add(vm);
                AllItems.Add(vm);
            }
        }

        public DirectoryItemViewModel(FileInfo file) : this() {
            Name = file.Name;
            VfsPath = ConvertSystemPathToVfsPath(file.FullName);

            Type = ObjectType.File;
        }

        private DirectoryItemViewModel() {
            OpenCommand = new RelayCommand(() => OnOpened?.Invoke(this));
            DeleteCommand = new RelayCommand(() => OnDeleted?.Invoke(this));
        }

        /// <summary>
        /// Called by the view when a file has been dropped onto it. Only works if this item is a directory.
        /// </summary>
        /// <param name="file">The full path of the file</param>
        public void OnFileDropped(string file) {
            Debug.Assert(Type == ObjectType.Directory, "Cannot perform a drop operation on a file");

            var fileInfo = new FileInfo(file);
            string originalPath = file;
            string newPath = Path.Combine(FullPath, fileInfo.Name);

            File.Copy(originalPath, newPath);
        }

        #region Item view model events
        private void OnItemOpened(DirectoryItemViewModel item) {
            //Not handled here. Just pipe up
            OnOpened?.Invoke(item);
        }

        private void OnItemDeleted(DirectoryItemViewModel item) {
            //Delete through the OS. The file system watcher will handle the event back to us.
            File.Delete(item.FullPath);
        }
        #endregion

        #region File system events
        private void OnFileCreated(object sender, FileSystemEventArgs e) {
            Application.Current.Dispatcher.Invoke(() => {
                DirectoryItemViewModel vm = File.GetAttributes(e.FullPath).HasFlag(FileAttributes.Directory)
                    ? CreateItem(new DirectoryInfo(e.FullPath))
                    : CreateItem(new FileInfo(e.FullPath));
                Files.Add(vm);
                AllItems.Add(vm);
            });
        }

        private void OnFileDeleted(object sender, FileSystemEventArgs e) {
            Application.Current.Dispatcher.Invoke(() => {
                DirectoryItemViewModel itemVm = null;
                foreach (var item in AllItems) {
                    if (item.Name == e.Name) {
                        itemVm = item;
                        break;
                    }
                }

                if (itemVm != null) {
                    DeleteItem(itemVm);
                }
            });
        }

        private void OnFileRenamed(object sender, RenamedEventArgs e) {
            Application.Current.Dispatcher.Invoke(() => {
                DirectoryItemViewModel itemVm = null;
                foreach (var item in AllItems) {
                    if (item.Name == e.OldName) {
                        itemVm = item;
                        break;
                    }
                }

                itemVm.Name = e.Name;
                itemVm.VfsPath = ConvertSystemPathToVfsPath(e.FullPath);
            });
        }
        #endregion

        private DirectoryItemViewModel CreateItem(DirectoryInfo directory) {
            var vm = new DirectoryItemViewModel(directory);
            vm.OnOpened += (DirectoryItemViewModel item) => OnItemOpened(item);
            vm.OnDeleted += (DirectoryItemViewModel item) => OnItemDeleted(item);
            vm.Parent = this;

            return vm;
        }

        private DirectoryItemViewModel CreateItem(FileInfo file) {
            var vm = new DirectoryItemViewModel(file);
            vm.OnOpened += (DirectoryItemViewModel item) => OnItemOpened(item);
            vm.OnDeleted += (DirectoryItemViewModel item) => OnItemDeleted(item);
            vm.Parent = this;

            return vm;
        }

        private void DeleteItem(DirectoryItemViewModel item) {
            AllItems.Remove(item);
            if (item.Type == ObjectType.Directory) {
                SubDirectories.Remove(item);
            } else {
                Files.Remove(item);
            }
        }

        private static string ConvertVfsPathToSystemPath(string vfsPath) => ((EditorApp)Application.Current).resolveVfsPath(vfsPath);

        private static string ConvertSystemPathToVfsPath(string systemPath) {
            string rootSystem = ConvertVfsPathToSystemPath(".");
            return systemPath.Remove(0, rootSystem.Length).Replace("\\", "/").Insert(0, ".");
        }
    }
}