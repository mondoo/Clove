using Membrane = garlic.membrane;

namespace Garlic.Bulb {
    class StaticModelComponentViewModel : ComponentViewModel {
        public string FilePath {
            get => filePath;
            set {
                filePath = value;
                OnPropertyChanged(nameof(filePath));
            }
        }
        private string filePath;

        public StaticModelComponentViewModel(string name, Membrane.ComponentType type) : base(name, type) { }
    }
}