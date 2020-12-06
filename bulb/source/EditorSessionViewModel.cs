using System;

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

        public EditorSessionViewModel()
        {
            Scene = new SceneViewModel();
            Scene.OnCreateEntity = CreateEntity;
        }

        private EntityViewModel CreateEntity()
        {
            var entityId = OnCreateEntity.Invoke();
            var entityVm = new EntityViewModel();

            entityVm.OnAddComponent = () => CreateComponent(entityId);

            return entityVm;
        }

        private ComponentViewModel CreateComponent(uint entityId)
        {
            Console.WriteLine($"Component created for: {entityId}");
            var vm = new ComponentViewModel();
            vm.Name = "Custom comp";

            return vm;
        }
    }
}