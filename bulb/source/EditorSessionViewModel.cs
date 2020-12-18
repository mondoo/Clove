using System;
using garlic.membrane;

namespace Garlic.Bulb
{
    /// <summary>
    /// The View Model for the entire editor's session. Used to manage the viewmodels that make up the editor itself.
    /// </summary>
    public class EditorSessionViewModel : ViewModel
    {
        public SceneViewModel Scene
        {
            get { return scene; }
            set
            {
                scene = value;
                OnPropertyChanged(nameof(Scene));
            }
        }
        private SceneViewModel scene;

        public LogViewModel Log { get; } = new LogViewModel();

        public delegate uint AddEntityEventHandler(); //Expecting a uint here as that's what a 
        public AddEntityEventHandler OnCreateEntity;

        public delegate void AddComponentEventHandler(uint entityId, ComponentType componentType);
        public AddComponentEventHandler OnCreateComponent;

        //TEMP: Brute force position changes
        public delegate void SetPositionEventHandler(uint entityId, float x, float y, float z);
        public SetPositionEventHandler OnSetPosition;

        public EditorSessionViewModel()
        {
            Scene = new SceneViewModel();
            Scene.OnCreateEntity = CreateEntity;
        }

        private EntityViewModel CreateEntity()
        {
            var entityId = OnCreateEntity.Invoke();
            var entityVm = new EntityViewModel();

            entityVm.OnAddComponent = (componentType) => CreateComponent(entityId, componentType);

            return entityVm;
        }

        private ComponentViewModel CreateComponent(uint entityId, ComponentType componentType)
        {
            ComponentViewModel vm;

            switch (componentType)
            {
                case ComponentType.Transform:
                    {
                        var transVm = new TransformComponentViewModel();
                        transVm.OnPositionChanged = (x, y, z) => OnSetPosition(entityId, x, y, z);
                        vm = transVm;
                    }
                    break;
                default:
                    vm = new ComponentViewModel();
                    break;
            }

            vm.Name = $"{componentType}";

            OnCreateComponent.Invoke(entityId, componentType);

            return vm;
        }
    }
}