using System.Windows;
using System.Runtime.InteropServices;

namespace Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("CloveWrapper")]
        private static extern void Set(int num);

        [DllImport("CloveWrapper")]
        private static extern int Get();

        public int Value {
            get {
                return Get();
            }
            set {
                Set(value);
            }
        }

        public MainWindow() {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e) {
            Value = 5;
            //Button.
            Button.Content = Value;
        }
    }
}
