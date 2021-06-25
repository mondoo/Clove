using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Input;

using Membrane = garlic.membrane;

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
        public string Name { get; }
        /// <summary>
        /// Full path of this item.
        /// </summary>
        public string FullName { get; }

        public ObjectType Type { get; }

        public DirectoryItemViewModel Parent { get; private set; }

        public ICommand OpenCommand { get; }

        public delegate void OpenHandler(DirectoryItemViewModel item);
        public OpenHandler OnOpened;

        /// <summary>
        /// A list of all directories within this directory.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> SubDirectories { get; } = new ObservableCollection<DirectoryItemViewModel>();

        /// <summary>
        /// A list of all files within this directory.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> Files { get; } = new ObservableCollection<DirectoryItemViewModel>();

        /// <summary>
        /// A list of every single item within this directory.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> AllItems { get; } = new ObservableCollection<DirectoryItemViewModel>();

        public DirectoryItemViewModel(string directoryPath) : this(new DirectoryInfo(directoryPath)) { }

        private FileSystemWatcher watcher;

        public DirectoryItemViewModel(DirectoryInfo directory) : this() {
            Name = directory.Name;
            FullName = directory.FullName;

            Type = ObjectType.Directory;

            watcher = new FileSystemWatcher(FullName, "*.*");
            watcher.EnableRaisingEvents = true;
            watcher.Created += OnFileCreated;
            //TODO: Other file system events

            foreach (var dir in directory.EnumerateDirectories()) {
                var vm = CreateItem(dir);

                SubDirectories.Add(vm);
                AllItems.Add(vm);
            }
            foreach (var file in directory.EnumerateFiles()) {
                var vm = CreateItem(file);

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
        /// Called by the view when a file has been dropped onto it
        /// </summary>
        /// <param name="file">The full path of the file</param>
        public void OnFileDropped(string file) {
            var fileInfo = new FileInfo(file);
            var message = new Membrane.Editor_CopyFile(); //TODO: Always copy or move files? We don't have a custom format for now so we don't need to perform any conversions

            message.originalPath = file;
            message.newPath = Path.Combine(FullName, fileInfo.Name);

            Membrane.MessageHandler.sendMessage(message);
        }

        private DirectoryItemViewModel() {
            OpenCommand = new RelayCommand(() => OnItemOpened(this));
        }

        private void OnItemOpened(DirectoryItemViewModel item) {
            OnOpened?.Invoke(item);
        }

        private void OnFileCreated(object sender, FileSystemEventArgs e) {
            //TODO: Assuming it's always a file
            var file = new FileInfo(e.FullPath);

            EditorApp.Current.Dispatcher.Invoke((Action)(() => {
                var vm = CreateItem(file);

                Files.Add(vm);
                AllItems.Add(vm);
            }));
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