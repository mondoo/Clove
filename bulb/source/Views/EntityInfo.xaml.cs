using System.Windows;
using System.Windows.Controls;

namespace Garlic.Bulb {
    /// <summary>
    /// A UserControl that displays infomation about an entity (name, components, etc.)
    /// </summary>
    public partial class EntityInfo : UserControl {
        public EntityInfo() {
            InitializeComponent();
        }

        private void TextBox_Drop(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.StringFormat)) {
                var textBox = (TextBlock)sender;
                string dataString = (string)e.Data.GetData(DataFormats.StringFormat);

                textBox.Text = dataString;

                e.Handled = true;
            }
        }
    }
}