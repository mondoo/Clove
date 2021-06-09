using System.Collections.ObjectModel;
using System.IO;

namespace Garlic.Bulb {
    public enum ObjectType {
        Directory,
        File
    }

    /// <summary>
    /// Contains information for a single Directory or File.
    /// </summary>
    public class DirectoryItemViewModel : ViewModel {
        public string Name { get; set; }
        public string Path { get; set; }
        public string Root { get; set; }
        public string Size { get; set; }
        public string Extension { get; set; }
        public ObjectType Type { get; set; }

        /// <summary>
        /// A list of all files and directories this item views.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> SubDirectories { get; } = new ObservableCollection<DirectoryItemViewModel>();

        public bool IsExpanded {
            get { return isExpanded; }
            set {
                isExpanded = value;
                OnPropertyChanged(nameof(IsExpanded));
            }
        }
        private bool isExpanded = false;

        public bool IsSelected {
            get { return isSelected; }
            set {
                isSelected = value;
                OnPropertyChanged(nameof(IsSelected));
            }
        }
        private bool isSelected = false;

        public DirectoryItemViewModel(DirectoryInfo directory) {
            Name = directory.Name;
            Root = directory.Root.Name;
            Path = directory.FullName;
            Type = ObjectType.Directory;

            foreach(var dir in directory.EnumerateDirectories()) {
                SubDirectories.Add(new DirectoryItemViewModel(dir));
            }
            foreach(var file in directory.EnumerateFiles()) {
                SubDirectories.Add(new DirectoryItemViewModel(file));
            }
        }

        public DirectoryItemViewModel(FileInfo file) {
            Name = file.Name;
            Path = file.FullName;
            Size = $"{file.Length / 1024} KB";
            Extension = file.Extension;
            Type = ObjectType.File;
        }
    }
}