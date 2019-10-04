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
        private static extern void OpenClove();

        public MainWindow() {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e) {
            try {
                OpenClove();
            } catch {
                Button.Content = "It broke";
            }
        }
    }
}
