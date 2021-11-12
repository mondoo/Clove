using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Input;

using Membrane = membrane;

namespace Bulb {
    public class EntityViewModel : ViewModel {
        public uint EntityId { get; set; }

        public string Name {
            get => name;
            set {
                name = value;
                OnPropertyChanged(nameof(Name));

                Membrane.MessageHandler.sendMessage(new Membrane.Editor_UpdateName {
                    entity = EntityId,
                    name = name
                });
            }
        }
        private string name = "New Entity";

        public ObservableCollection<ComponentViewModel> Components { get; } = new ObservableCollection<ComponentViewModel>();
        public ObservableCollection<ComponentMenuItemViewModel> ComponentMenuItems { get; } = new ObservableCollection<ComponentMenuItemViewModel>();

        public ICommand SelectedCommand { get; }

        public delegate void SelectionHandler(EntityViewModel viewModel);
        public SelectionHandler OnSelected;

        public EntityViewModel() {
            //Bind to messages
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnComponentAdded>(OnComponentCreated);
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnComponentRemoved>(OnComponentRemoved);

            //Set up commands
            SelectedCommand = new RelayCommand(() => OnSelected?.Invoke(this));

            RefreshAvailableComponents();
        }

        public EntityViewModel(List<Membrane.Component> components) : this() {
            //foreach (Membrane.Component component in components) {
            //    Components.Add(CreateComponentViewModel(component.type, component.initData));
            //}
            RefreshAvailableComponents();
        }

        private void AddComponent(string typeName) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_AddComponent {
                entity = EntityId,
                componentName = typeName
            });
        }

        private void ModifyComponent(string typeName, byte[] componentData) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_ModifyComponent {
                entity = EntityId,
                componentName = typeName,
                data = componentData
            });
        }

        private void RemoveComponent(string typeName) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_RemoveComponent {
                entity = EntityId,
                componentName = typeName
            });
        }

        private void OnComponentCreated(Membrane.Engine_OnComponentAdded message) {
            if (EntityId == message.entity) {
                var vm = new ComponentViewModel(Membrane.ReflectionHelper.getInfoForType(message.componentName), message.data);
                vm.OnModified = ModifyComponent;
                vm.OnRemoved = RemoveComponent;

                Components.Add(vm);

                RefreshAvailableComponents();
            }
        }

        private void OnComponentRemoved(Membrane.Engine_OnComponentRemoved message) {
            if (EntityId == message.entity) {
                foreach (ComponentViewModel component in Components) {
                    if (component.TypeName == message.componentName) {
                        Components.Remove(component);
                        break;
                    }
                }

                RefreshAvailableComponents();
            }
        }

        private void RefreshAvailableComponents() {
            List<string> entitiesComponents = new List<string>();
            foreach (ComponentViewModel component in Components) {
                entitiesComponents.Add(component.Name);
            }

            ComponentMenuItems.Clear();

            List<Membrane.EditorTypeInfo> types = Membrane.ReflectionHelper.getAvailableTypes();

            foreach (Membrane.EditorTypeInfo type in types) {
                if (!entitiesComponents.Contains(type.displayName)) {
                    ComponentMenuItems.Add(new ComponentMenuItemViewModel(type.displayName, new RelayCommand(() => AddComponent(type.typeName))));
                }
            }
        }
    }
}