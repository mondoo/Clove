namespace Garlic.Bulb
{
    /// <summary>
    /// The base viewmodel for all components
    /// </summary>
    public class ComponentViewModel : ViewModel
    {
        public string Name { get; set; }
    }

    //TEMP
    public class TransformComponentViewModel : ComponentViewModel
    {
        public string XText { get; } = "X";
        public string XValue
        {
            get { return xValue; }
            set { xValue = value; OnPropertyChanged(nameof(xValue)); }
        }
        private string xValue = "0";

        public string YText { get; } = "Y";
        public string YValue
        {
            get { return yValue; }
            set { yValue = value; OnPropertyChanged(nameof(yValue)); }
        }
        private string yValue = "0";

        public string ZText { get; } = "Z";
        public string ZValue
        {
            get { return zValue; }
            set { zValue = value; OnPropertyChanged(nameof(zValue)); }
        }
        private string zValue = "0";
    }
}