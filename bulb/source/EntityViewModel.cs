using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;

using Membrane = garlic.membrane;

namespace Garlic.Bulb
{
    public class EntityViewModel : ViewModel
    {
        public uint EntityId{ get; set; }
        
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

            //Set up commands
            SelectedCommand = new RelayCommand(() => OnSelected?.Invoke(this));
        }

        public void AddComponent(Membrane.ComponentType type) {
            var message = new Membrane.Editor_CreateComponent();
            message.entity = EntityId;
            message.componentType = type;

            Membrane.MessageHandler.sendMessage(message);
        }

        private void OnComponentCreated(Membrane.Engine_OnComponentCreated message){
            if(EntityId == message.entity){
                ComponentViewModel componentVm;

                switch(message.componentType){
                    case Membrane.ComponentType.Transform:
                        componentVm = new TransformComponentViewModel();
                        break;
                    default:
                        componentVm = new ComponentViewModel();
                        break;
                }

                componentVm.Name = $"{message.componentType}";

                Components.Add(componentVm);
            }
        }
    }
}