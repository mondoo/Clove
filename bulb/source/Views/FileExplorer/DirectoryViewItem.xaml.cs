using System.Windows.Controls;

namespace Garlic.Bulb {
    /// <summary>
    /// A UserControl that displays the entire contents of a single directory in a grid.
    /// Expects a DirectoryViewModel
    /// </summary>
    public partial class DirectoryViewItem : UserControl {
        public DirectoryViewItem() {
            InitializeComponent();
        }
    }
}