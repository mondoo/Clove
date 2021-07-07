using Membrane = garlic.membrane;

namespace Garlic.Bulb {
    class StaticModelComponentViewModel : ComponentViewModel {
        public string FilePath {
            get => filePath;
            set {
                filePath = value;
                OnPropertyChanged(nameof(FilePath));
                OnStaticModelChanged?.Invoke(filePath);
            }
        }
        private string filePath = "";

        public delegate void StaticModelChangedHandler(string path);
        public StaticModelChangedHandler OnStaticModelChanged;

        public StaticModelComponentViewModel(string name, Membrane.ComponentType type) : base(name, type) { }
    }
}