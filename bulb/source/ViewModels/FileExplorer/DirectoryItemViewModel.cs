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
        public string Path { get; }
        public string Root { get; }
        public string Size { get; }
        public string Extension { get; }
        public ObjectType Type { get; }

        public ICommand OpenCommand;

        public delegate void OpenHandler();
        public OpenHandler OnOpened;

        /// <summary>
        /// A list of all directories this item views.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> SubDirectories { get; } = new ObservableCollection<DirectoryItemViewModel>();
        
        /// <summary>
        /// A list of all files this item views.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> Files { get; } = new ObservableCollection<DirectoryItemViewModel>();

        public DirectoryItemViewModel(string directoryPath) : this(new DirectoryInfo(directoryPath)) { }

        public DirectoryItemViewModel(DirectoryInfo directory) : this() {
            Name = directory.Name;
            Root = directory.Root.Name;
            Path = directory.FullName;
            Type = ObjectType.Directory;

            foreach (var dir in directory.EnumerateDirectories()) {
                SubDirectories.Add(new DirectoryItemViewModel(dir));
                //TODO: OnOpened
            }
            foreach (var file in directory.EnumerateFiles()) {
                Files.Add(new DirectoryItemViewModel(file));
                //TODO: OnOpened
            }
        }

        public DirectoryItemViewModel(FileInfo file) : this() {
            Name = file.Name;
            Path = file.FullName;
            Size = $"{file.Length / 1024} KB";
            Extension = file.Extension;
            Type = ObjectType.File;
        }

        private DirectoryItemViewModel() {
            OpenCommand = new RelayCommand(() => OnOpened?.Invoke());
        }
    }
}