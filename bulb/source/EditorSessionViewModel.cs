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
            get { return sceneViewModel; }
            set
            {
                sceneViewModel = value;
                OnPropertyChanged(nameof(Scene));
            }
        }
        private SceneViewModel sceneViewModel;

        public delegate uint AddEntityEventHandler(); //Expecting a uint here as that's what a 
        public AddEntityEventHandler OnCreateEntity;

        public EditorSessionViewModel()
        {
            Scene = new SceneViewModel();
            Scene.OnCreateEntity += () =>
            {
                var entityId = OnCreateEntity?.Invoke();
                var entityVm = new EntityViewModel();

                entityVm.OnAddComponent = () => Console.WriteLine("Component added to (id): " + entityId);

                return entityVm;
            };
        }
    }
}