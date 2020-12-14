namespace Garlic.Bulb
{
    /// <summary>
    /// Viewmodel representing a Transform Component
    /// </summary>
    public class TransformComponentViewModel : ComponentViewModel
    {
        public string XText { get; } = "X";
        public string XValue
        {
            get { return xValue.ToString(); }
            set
            {
                double number;
                if (double.TryParse(value, out number))
                {
                    xValue = number;
                }
                //Broadcast even if it isn't a match to reset the view back to the last valid number
                OnPropertyChanged(nameof(xValue));
            }
        }
        private double xValue = 0;

        public string YText { get; } = "Y";
        public string YValue
        {
            get { return yValue.ToString(); }
            set
            {
                double number;
                if (double.TryParse(value, out number))
                {
                    yValue = number;
                }
                //Broadcast even if it isn't a match to reset the view back to the last valid number
                OnPropertyChanged(nameof(yValue));
            }
        }
        private double yValue = 0;

        public string ZText { get; } = "Z";
        public string ZValue
        {
            get { return zValue.ToString(); }
            set
            {
                double number;
                if (double.TryParse(value, out number))
                {
                    zValue = number;
                }
                //Broadcast even if it isn't a match to reset the view back to the last valid number
                OnPropertyChanged(nameof(zValue));
            }
        }
        private double zValue = 0;
    }
}