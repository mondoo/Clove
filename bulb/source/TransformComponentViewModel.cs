namespace Garlic.Bulb
{
    /// <summary>
    /// Viewmodel representing a Transform Component
    /// </summary>
    public class TransformComponentViewModel : ComponentViewModel
    {
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
                OnPositionChanged?.Invoke(xValue, yValue, zValue);
            }
        }
        private float xValue = 0;

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
                OnPositionChanged?.Invoke(xValue, yValue, zValue);
            }
        }
        private float yValue = 0;

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
                OnPositionChanged?.Invoke(xValue, yValue, zValue);
            }
        }
        private float zValue = 0;

        public delegate void PositionChangedHandler(float x, float y, float z);
        public PositionChangedHandler OnPositionChanged;

        public void SetPosition(float x, float y, float z){
            xValue = x;
            yValue = y;
            zValue = z;

            OnPropertyChanged(nameof(XValue));
            OnPropertyChanged(nameof(YValue));
            OnPropertyChanged(nameof(ZValue));
        }
    }
}