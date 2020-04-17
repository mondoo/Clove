using System;
using System.Text.RegularExpressions;
using System.Windows.Controls;
using System.Windows.Input;

namespace Bulb.Windows
{
	/// <summary>
	/// Interaction logic for TransformComponent3D.xaml
	/// </summary>
	public partial class TransformComponent3D : UserControl {
		public TransformComponent3D() {
			InitializeComponent();
		}

		private void TextBox_ValidateNumber(object sender, TextCompositionEventArgs e) {
			Regex regex = new Regex("[^0-9.-]+");
			e.Handled = regex.IsMatch(e.Text);
		}
	}
}
