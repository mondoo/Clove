using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Input;
using System.Windows;

using Membrane = garlic.membrane;

namespace Garlic.Bulb
{
    /// <summary>
    /// The View Model for the editor's current scene.
    /// </summary>
    public class SceneViewModel : ViewModel
    {
        public ICommand CreateEntityCommand { get; }
        public ICommand AddTransformComponentCommand { get; }
        public ICommand AddMeshComponentCommand { get; }

        public ObservableCollection<EntityViewModel> Entities { get; } = new ObservableCollection<EntityViewModel>();

        public EntityViewModel SelectedEntity
        {
            get { return selectedEntity; }
            set
            {
                selectedEntity = value;
                OnPropertyChanged(nameof(SelectedEntity));
                OnPropertyChanged(nameof(EntityViewVisibility));
            }
        }
        private EntityViewModel selectedEntity;

        public Visibility EntityViewVisibility
        {
            get
            {
                return selectedEntity != null ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        public SceneViewModel()
        {
            //Bind to messages
            Membrane.MessageHandler.bindToMessage<Membrane.Engine_OnEntityCreated>(OnEntityCreated);

            //Set up commands
            CreateEntityCommand = new RelayCommand(() => Membrane.MessageHandler.sendMessage(new Membrane.Editor_CreateEntity()));
            AddTransformComponentCommand = new RelayCommand(() => SelectedEntity?.AddComponent(Membrane.ComponentType.Transform));
            AddMeshComponentCommand = new RelayCommand(() => SelectedEntity?.AddComponent(Membrane.ComponentType.Mesh));
        }

        private void OnEntityCreated(Membrane.Engine_OnEntityCreated message){
            var entityVm = new EntityViewModel();
            entityVm.EntityId = message.entity;
            entityVm.Name = message.name;
            entityVm.OnSelected = (EntityViewModel viewModel) => SelectedEntity = viewModel;

            Entities.Add(entityVm);
        }
    };
}