using System;
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
            set {
                name = value;
                OnPropertyChanged(nameof(Name));
            }
        }
        private string name;

        /// <summary>
        /// Full path of this item.
        /// </summary>
        public string FullName { get; set; }

        public ObjectType Type { get; }

        /// <summary>
        /// Parent directory of this item.
        /// </summary>
        public DirectoryItemViewModel Parent { get; private set; }

        public ICommand OpenCommand { get; }

        public delegate void OpenHandler(DirectoryItemViewModel item);
        public OpenHandler OnOpened;

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

        public DirectoryItemViewModel(string directoryPath) : this(new DirectoryInfo(directoryPath)) { }

        private FileSystemWatcher watcher;

        public DirectoryItemViewModel(DirectoryInfo directory) : this() {
            Name = directory.Name;
            FullName = directory.FullName;

            Type = ObjectType.Directory;

            watcher = new FileSystemWatcher(FullName, "*.*") {
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
            FullName = file.FullName;

            Type = ObjectType.File;
        }

        /// <summary>
        /// Called by the view when a file has been dropped onto it. Only works if this item is a directory.
        /// </summary>
        /// <param name="file">The full path of the file</param>
        public void OnFileDropped(string file) {
            Debug.Assert(Type == ObjectType.Directory, "Cannot performa a drop operation on a file");

            var fileInfo = new FileInfo(file);
            string originalPath = file;
            string newPath = Path.Combine(FullName, fileInfo.Name);

            File.Copy(originalPath, newPath);
        }

        private DirectoryItemViewModel() {
            OpenCommand = new RelayCommand(() => OnItemOpened(this));
        }

        private void OnItemOpened(DirectoryItemViewModel item) {
            OnOpened?.Invoke(item);
        }

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
                    AllItems.Remove(itemVm);
                    //Item no longer exists to quiery for data so try to remove from both lists
                    SubDirectories.Remove(itemVm);
                    Files.Remove(itemVm);
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
                itemVm.FullName = e.FullPath;
            });
        }

        private DirectoryItemViewModel CreateItem(DirectoryInfo directory) {
            var vm = new DirectoryItemViewModel(directory);
            vm.OnOpened += (DirectoryItemViewModel item) => OnItemOpened(item);
            vm.Parent = this;

            return vm;
        }

        private DirectoryItemViewModel CreateItem(FileInfo file) {
            var vm = new DirectoryItemViewModel(file);
            vm.OnOpened += (DirectoryItemViewModel item) => OnItemOpened(item);
            vm.Parent = this;

            return vm;
        }
    }
}