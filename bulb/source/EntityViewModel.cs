using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;

using Membrane = garlic.membrane;

namespace Garlic.Bulb
{
    public class EntityViewModel : ViewModel
    {
        public uint EntityId { get; set; }

        public string Name
        {
            get { return name; }
            set
            {
                name = value;
                OnPropertyChanged(nameof(Name));
            }
        }
        private string name = "New Entity";

        public ObservableCollection<ComponentViewModel> Components { get; } = new ObservableCollection<ComponentViewModel>();

        public ICommand SelectedCommand { get; }

        public delegate void SelectionHandler(EntityViewModel viewModel);
        public SelectionHandler OnSelected;

        public EntityViewModel()
        {
            //Bind to messages
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnComponentCreated>(OnComponentCreated);
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnTransformChanged>(OnTransformChanged);

            //Set up commands
            SelectedCommand = new RelayCommand(() => OnSelected?.Invoke(this));
        }

        public void AddComponent(Membrane.ComponentType type)
        {
            var message = new Membrane.Editor_CreateComponent();
            message.entity = EntityId;
            message.componentType = type;

            Membrane.MessageHandler.sendMessage(message);
        }

        private void OnComponentCreated(Membrane.Engine_OnComponentCreated message)
        {
            if (EntityId == message.entity)
            {
                ComponentViewModel componentVm;

                switch (message.componentType)
                {
                    case Membrane.ComponentType.Transform:
                        {
                            var transformComp = new TransformComponentViewModel();
                            transformComp.OnPositionChanged = UpdateTransform;

                            componentVm = transformComp;
                        }
                        break;
                    default:
                        componentVm = new ComponentViewModel();
                        break;
                }

                componentVm.Name = $"{message.componentType}";

                Components.Add(componentVm);
            }
        }

        private void UpdateTransform(float x, float y, float z)
        {
            var message = new Membrane.Editor_UpdateTransform();
            message.entity = EntityId;
            message.position = new Membrane.Vector3(x, y, z);

            Membrane.MessageHandler.sendMessage(message);
        }

        private void OnTransformChanged(Membrane.Engine_OnTransformChanged message)
        {
            if (EntityId == message.entity)
            {
                foreach (ComponentViewModel comp in Components)
                {
                    if (comp.GetType() == typeof(TransformComponentViewModel))
                    {
                        ((TransformComponentViewModel)comp).SetPosition(message.position.x, message.position.y, message.position.z);
                        break;
                    }
                }
            }
        }
    }
}