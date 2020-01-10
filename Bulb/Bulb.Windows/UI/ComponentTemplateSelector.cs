using Bulb.Core;
using Bulb.UI;
using System.Windows;
using System.Windows.Controls;

namespace Bulb {
	public class ComponentTemplateSelector : DataTemplateSelector {
		public DataTemplate Unkown { get; set; }
		public DataTemplate TransformComponent { get; set; }
		public DataTemplate MeshComponent { get; set; }

		public override DataTemplate SelectTemplate(object item, DependencyObject container) {
			var component = item as ComponentViewModel;
			switch(component.Type) {
				case ComponentType.Transform:
					return TransformComponent;
				case ComponentType.Mesh:
					return MeshComponent;
				default:
					return Unkown;
			}
		}
	}
}