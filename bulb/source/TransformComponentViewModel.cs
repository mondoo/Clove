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
                float number;
                if (float.TryParse(value, out number))
                {
                    xValue = number;
                }
                //Broadcast even if it isn't a match to reset the view back to the last valid number
                OnPropertyChanged(nameof(xValue));
                OnPositionChanged?.Invoke(xValue, yValue, zValue);
            }
        }
        private float xValue = 0;

        public string YText { get; } = "Y";
        public string YValue
        {
            get { return yValue.ToString(); }
            set
            {
                float number;
                if (float.TryParse(value, out number))
                {
                    yValue = number;
                }
                //Broadcast even if it isn't a match to reset the view back to the last valid number
                OnPropertyChanged(nameof(yValue));
                OnPositionChanged?.Invoke(xValue, yValue, zValue);
            }
        }
        private float yValue = 0;

        public string ZText { get; } = "Z";
        public string ZValue
        {
            get { return zValue.ToString(); }
            set
            {
                float number;
                if (float.TryParse(value, out number))
                {
                    zValue = number;
                }
                //Broadcast even if it isn't a match to reset the view back to the last valid number
                OnPropertyChanged(nameof(zValue));
                OnPositionChanged?.Invoke(xValue, yValue, zValue);
            }
        }
        private float zValue = 0;

        public delegate void PositionChangedHandler(float x, float y, float z);
        public PositionChangedHandler OnPositionChanged;

        // public void SetPosition(float x, float y, float z){
        //     xValue = x;
        //     yValue = y;
        //     zValue = z;
        // }
    }
}