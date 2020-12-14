namespace Garlic.Bulb
{
    /// <summary>
    /// The base viewmodel for all components
    /// </summary>
    public class ComponentViewModel : ViewModel
    {
        public string Name
        {
            get { return name; }
            set { name = value; OnPropertyChanged(nameof(name)); }
        }
        private string name = "";
    }
}