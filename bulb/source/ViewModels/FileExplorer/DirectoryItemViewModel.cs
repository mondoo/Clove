using System.Collections.ObjectModel;
using System.IO;
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
        public string Name { get; }
        public ObjectType Type { get; }

        public ICommand OpenCommand;

        public delegate void OpenHandler();
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

        public DirectoryItemViewModel(DirectoryInfo directory) : this() {
            Name = directory.Name;
            Type = ObjectType.Directory;

            foreach (var dir in directory.EnumerateDirectories()) {
                var vm = new DirectoryItemViewModel(dir);
                
                SubDirectories.Add(vm);
                AllItems.Add(vm);
                //TODO: OnOpened
            }
            foreach (var file in directory.EnumerateFiles()) {
                var vm = new DirectoryItemViewModel(file);

                Files.Add(vm);
                AllItems.Add(vm);
                //TODO: OnOpened
            }
        }

        public DirectoryItemViewModel(FileInfo file) : this() {
            Name = file.Name;
            Type = ObjectType.File;
        }

        private DirectoryItemViewModel() {
            OpenCommand = new RelayCommand(() => OnOpened?.Invoke());
        }
    }
}