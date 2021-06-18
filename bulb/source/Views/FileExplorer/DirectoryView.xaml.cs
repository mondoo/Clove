using System.Windows.Controls;

namespace Garlic.Bulb {
    /// <summary>
    /// A UserControl that displays the entire contents of a single directory in a grid.
    /// Expects a DirectoryViewModel
    /// </summary>
    public partial class DirectoryView : UserControl {
        public DirectoryView() {
            InitializeComponent();
        }
    }
}