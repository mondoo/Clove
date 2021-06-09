using System.Collections.ObjectModel;

namespace Garlic.Bulb {
    /// <summary>
    /// Contains information for an entire single directory.
    /// </summary>
    public class DirectoryViewModel : ViewModel {
        /// <summary>
        /// The currently selected item.
        /// </summary>
        public DirectoryItemViewModel CurrentItem {
            get { return currentItem; }
            set {
                currentItem = value;
                OnPropertyChanged(nameof(CurrentItem));
            }
        }
        private DirectoryItemViewModel currentItem;

        /// <summary>
        /// All of the items at the root of this directory.
        /// </summary>
        public ObservableCollection<DirectoryItemViewModel> Items { get; }

        public delegate void ItemOpenHandler(DirectoryItemViewModel item);
        public ItemOpenHandler OnItemOpened;

        public DirectoryViewModel(DirectoryItemViewModel rootDirectory) {
            Items = rootDirectory.SubDirectories;
        }

        public void OpenCurrentItem() {
            OnItemOpened?.Invoke(CurrentItem);
        }
    }
}