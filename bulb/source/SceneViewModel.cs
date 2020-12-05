using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Input;
using System.Windows;

namespace Garlic.Bulb
{
    /// <summary>
    /// The View Model for the editor's current scene.
    /// </summary>
    public class SceneViewModel : ViewModel
    {
        public ICommand CreateEntityCommand { get; }
        public ICommand AddComponentCommand { get; }

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

        public delegate EntityViewModel AddEntityEventHandler();
        public AddEntityEventHandler OnCreateEntity;

        public SceneViewModel()
        {
            CreateEntityCommand = new RelayCommand(() =>
            {
                if (OnCreateEntity != null)
                {
                    var entityVM = OnCreateEntity.Invoke();
                    entityVM.OnSelected = (EntityViewModel viewModel) => SelectedEntity = viewModel;

                    Entities.Add(entityVM);
                }
            });

            AddComponentCommand = new RelayCommand(() => SelectedEntity?.AddComponent());
        }
    };
}