using System.ComponentModel;
using System.Windows.Controls;

namespace Garlic.Bulb
{
    /// <summary>
    /// A UserControl for displaying 3 component vectors
    /// </summary>
    public partial class Vector3Box : UserControl, INotifyPropertyChanged {
        public string XText { get { return xtext; } set {
                xtext = value;
                OnPropertyChanged(nameof(XText));
            }
        }
        private string xtext = "x";
        public string XValue { get; set; } = "0";

        public string YText { get; set; } = "Y";
        public string YValue { get; set; } = "0";

        public string ZText { get; set; } = "Z";
        public string ZValue { get; set; } = "0";

        public event PropertyChangedEventHandler PropertyChanged = (sender, e) => { };

        public Vector3Box(){
            InitializeComponent();
        }

        protected void OnPropertyChanged(string propertyName) => PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        
    }
}