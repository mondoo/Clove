using Membrane = garlic.membrane;

namespace Garlic.Bulb
{
    /// <summary>
    /// Viewmodel representing a Transform Component
    /// </summary>
    public class TransformComponentViewModel : ComponentViewModel
    {
        public string XValue
        {
            get { return position.x.ToString(); }
            set
            {
                float number;
                float.TryParse(value, out number);
                var temp = position;
                temp.x = number;
                OnTransformChanged?.Invoke(temp, rotation, scale);
            }
        }

        public string YValue
        {
            get { return position.y.ToString(); }
            set
            {
                float number;
                float.TryParse(value, out number);
                var temp = position;
                temp.y = number;
                OnTransformChanged?.Invoke(temp, rotation, scale);
            }
        }

        public string ZValue
        {
            get { return position.z.ToString(); }
            set
            {
                float number;
                float.TryParse(value, out number);
                var temp = position;
                temp.z = number;
                OnTransformChanged?.Invoke(temp, rotation, scale);
            }
        }

        Membrane.Vector3 position = new Membrane.Vector3();
        Membrane.Vector3 rotation = new Membrane.Vector3();
        Membrane.Vector3 scale = new Membrane.Vector3(1, 1, 1);

        public delegate void TransformChangedHandler(Membrane.Vector3 position, Membrane.Vector3 rotation, Membrane.Vector3 scale);
        public TransformChangedHandler OnTransformChanged;

        public void Update(Membrane.Vector3 position, Membrane.Vector3 rotation, Membrane.Vector3 scale){
            this.position = position;

            OnPropertyChanged(nameof(XValue));
            OnPropertyChanged(nameof(YValue));
            OnPropertyChanged(nameof(ZValue));
        }
    }
}