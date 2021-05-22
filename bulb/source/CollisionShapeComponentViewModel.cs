using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using Membrane = garlic.membrane;

namespace Garlic.Bulb {
    public class CollisionShapeComponentViewModel : ComponentViewModel {
        public enum ShapeType {
            Sphere,
            Cube
        };

        public class ShapeTypeViewModel {
            public string Name { get; }
            public ShapeType Type { get; }
            public ICommand OnSelectedCommand { get; }

            public ShapeTypeViewModel(string name, ShapeType type, ICommand onSelectedCommand) {
                Name = name;
                Type = type;
                OnSelectedCommand = onSelectedCommand;
            }
        }

        public float Radius { get; set; }
        public Membrane.Vector3 HalfExtents { get; set; }

        public ShapeType CurrentShapeType {
            get { return currentShapeType; }
            private set {
                currentShapeType = value;
                OnPropertyChanged(nameof(CurrentShapeType));
            }
        }
        private ShapeType currentShapeType;

        public ObservableCollection<ShapeTypeViewModel> AvailableShapes { get; } = new ObservableCollection<ShapeTypeViewModel>();

        public Visibility RadiusVisibility {
            get { return radiusVisibility; }
            private set {
                radiusVisibility = value;
                OnPropertyChanged(nameof(RadiusVisibility));
            }
        }
        private Visibility radiusVisibility = Visibility.Visible;

        public Visibility HalfExtentsVisibility {
            get { return halfExtentsVisibility; }
            private set {
                halfExtentsVisibility = value;
                OnPropertyChanged(nameof(HalfExtentsVisibility));
            }
        }
        private Visibility halfExtentsVisibility = Visibility.Visible;

        public CollisionShapeComponentViewModel(string name, Membrane.ComponentType type) : base(name, type) {
            //Start off as a sphere
            OnShapeTypeChanged(ShapeType.Sphere);
        }

        private void OnShapeTypeChanged(ShapeType newType) {
            CurrentShapeType = newType;

            switch (CurrentShapeType) {
                case ShapeType.Sphere:
                    RadiusVisibility = Visibility.Visible;
                    HalfExtentsVisibility = Visibility.Collapsed;
                    break;
                case ShapeType.Cube:
                    RadiusVisibility = Visibility.Collapsed;
                    HalfExtentsVisibility = Visibility.Visible;
                    break;
            }

            //Update the available shapes to remove the selected one
            var shapeTypes = Enum.GetValues(typeof(ShapeType)).Cast<ShapeType>();

            AvailableShapes.Clear();
            foreach (ShapeType shape in shapeTypes) {
                if (shape != CurrentShapeType) {
                    AvailableShapes.Add(new ShapeTypeViewModel($"{shape}", shape, new RelayCommand(() => OnShapeTypeChanged(shape))));
                }
            }
        }
    }
}