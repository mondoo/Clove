using System.Windows;
using System.Windows.Controls;

namespace Garlic.Bulb
{
    /// <summary>
    /// A UserControl for displaying 3 component vectors
    /// </summary>
    public partial class Vector3Box : UserControl
    {
        public string LeftFieldText { get; set; }
        public string LeftFieldValue { get; set; }

        public string CenterFieldText { get; set; }
        public string CenterFieldValue { get; set; }

        public string RightFieldText { get; set; }
        public string RightFieldValue { get; set; }

        public static DependencyProperty XTextProperty = DependencyProperty.Register(nameof(LeftFieldText), typeof(object), typeof(Vector3Box), new PropertyMetadata(null));
        public static DependencyProperty XValueProperty = DependencyProperty.Register(nameof(LeftFieldValue), typeof(object), typeof(Vector3Box), new PropertyMetadata(null));

        public static DependencyProperty YTextProperty = DependencyProperty.Register(nameof(CenterFieldText), typeof(object), typeof(Vector3Box), new PropertyMetadata(null));
        public static DependencyProperty YValueProperty = DependencyProperty.Register(nameof(CenterFieldValue), typeof(object), typeof(Vector3Box), new PropertyMetadata(null));

        public static DependencyProperty ZTextProperty = DependencyProperty.Register(nameof(RightFieldText), typeof(object), typeof(Vector3Box), new PropertyMetadata(null));
        public static DependencyProperty ZValueProperty = DependencyProperty.Register(nameof(RightFieldValue), typeof(object), typeof(Vector3Box), new PropertyMetadata(null));

        public Vector3Box()
        {
            InitializeComponent();
        }
    }
}