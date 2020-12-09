using System.Windows.Controls;

namespace Garlic.Bulb
{
    /// <summary>
    /// A UserControl for displaying 3 component vectors
    /// </summary>
    public partial class Vector3Box : UserControl
    {
        public Vector3Box(){
            InitializeComponent();
        }

        public string XText { get; set; } = "X";
        public string XValue { get; set; } = "0";

        public string YText { get; set; } = "Y";
        public string YValue { get; set; } = "0";

        public string ZText { get; set; } = "Z";
        public string ZValue { get; set; } = "0";
    }
}