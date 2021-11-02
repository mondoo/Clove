using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
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

            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnRigidBodyChanged>(OnRigidBodyChanged);
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnTransformChanged>(OnTransformChanged);
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnSphereShapeChanged>(OnSphereShapeChanged);
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnCubeShapeChanged>(OnCubeShapeChanged);

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

        public void AddComponent(/*Membrane.ComponentType type*/) {
            //Membrane.MessageHandler.sendMessage(new Membrane.Editor_AddComponent {
            //    entity = EntityId,
            //    componentType = type
            //});
        }

        public void RemoveComponent(/*Membrane.ComponentType type*/) {
            //Membrane.MessageHandler.sendMessage(new Membrane.Editor_RemoveComponent {
            //    entity = EntityId,
            //    componentType = type
            //});
        }

        private void OnComponentCreated(Membrane.Engine_OnComponentAdded message) {
            if (EntityId == message.entity) {
                Components.Add(new ComponentViewModel(message.componentName));
                RefreshAvailableComponents();
            }
        }

        private void OnComponentRemoved(Membrane.Engine_OnComponentRemoved message) {
            //if (EntityId == message.entity) {
            //    foreach (ComponentViewModel component in Components) {
            //        if (component.Type == message.componentType) {
            //            Components.Remove(component);
            //            break;
            //        }
            //    }

            //    RefreshAvailableComponents();
            //}
        }

        private void RefreshAvailableComponents() {
            List<string> entitiesComponents = new List<string>();
            foreach (ComponentViewModel component in Components) {
                entitiesComponents.Add(component.Name);
            }

            ComponentMenuItems.Clear();

            //TODO: Filter based on what the entity already has.
            List<Membrane.EditorTypeInfo> types = Membrane.ReflectionHelper.getAvailableTypes();

            foreach (Membrane.EditorTypeInfo type in types) {
                if (!entitiesComponents.Contains(type.displayName)) {
                    ComponentMenuItems.Add(new ComponentMenuItemViewModel(type.displayName, new RelayCommand(() => {
                        Membrane.MessageHandler.sendMessage(new Membrane.Editor_AddComponent {
                            entity = EntityId,
                            typeName = type.typeName,
                        });
                    })));
                }
            }
        }

        private void UpdateTransform(Membrane.Vector3 position, Membrane.Vector3 rotation, Membrane.Vector3 scale) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_UpdateTransform {
                entity = EntityId,
                position = position,
                rotation = rotation,
                scale = scale
            });
        }

        private void UpdateStaticModel(string meshPath, string diffusePath, string specularPath) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_UpdateStaticModel {
                entity = EntityId,
                meshPath = meshPath,
                diffusePath = diffusePath,
                specularPath = specularPath
            });
        }

        private void UpdateRigidBody(float mass) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_UpdateRigidBody {
                entity = EntityId,
                mass = mass
            });
        }

        private void UpdateSphereShape(float radius) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_UpdateSphereShape {
                entity = EntityId,
                radius = radius
            });
        }

        private void UpdateCubeShape(Membrane.Vector3 halfExtents) {
            Membrane.MessageHandler.sendMessage(new Membrane.Editor_UpdateCubeShape {
                entity = EntityId,
                halfExtents = halfExtents
            });
        }

        private void OnRigidBodyChanged(Membrane.Engine_OnRigidBodyChanged message) {
            //if (EntityId == message.entity) {
            //    foreach (ComponentViewModel comp in Components) {
            //        if (comp.GetType() == typeof(RigidBodyComponentViewModel)) {
            //            var rigidBody = comp as RigidBodyComponentViewModel;
            //            rigidBody.Update(message.mass);
            //            break;
            //        }
            //    }
            //}
        }

        private void OnTransformChanged(Membrane.Engine_OnTransformChanged message) {
            //if (EntityId == message.entity) {
            //    foreach (ComponentViewModel comp in Components) {
            //        if (comp.GetType() == typeof(TransformComponentViewModel)) {
            //            var transform = comp as TransformComponentViewModel;
            //            transform.Update(message.position, message.rotation, message.scale);
            //            break;
            //        }
            //    }
            //}
        }

        private void OnSphereShapeChanged(Membrane.Engine_OnSphereShapeChanged message) {
            //if (EntityId == message.entity) {
            //    foreach (ComponentViewModel comp in Components) {
            //        if (comp.GetType() == typeof(CollisionShapeComponentViewModel)) {
            //            var collisionShape = comp as CollisionShapeComponentViewModel;
            //            collisionShape.UpdateSphereShape(message.radius);
            //            break;
            //        }
            //    }
            //}
        }

        private void OnCubeShapeChanged(Membrane.Engine_OnCubeShapeChanged message) {
            //if (EntityId == message.entity) {
            //    foreach (ComponentViewModel comp in Components) {
            //        if (comp.GetType() == typeof(CollisionShapeComponentViewModel)) {
            //            var collisionShape = comp as CollisionShapeComponentViewModel;
            //            collisionShape.UpdateCubeShape(message.halfExtents);
            //            break;
            //        }
            //    }
            //}
        }
    }
}