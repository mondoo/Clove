using Microsoft.Win32;
using System.IO;
using System.Windows;

using Membrane = membrane;

namespace Bulb {
    public struct ProjectSelectedArgs {
        public string ProjectPath; //Just the name of the project (file name without extension)
        public string ProjectName;
        public string FullPath; //Full parth of the project
        public bool IsNewProject;
    }

    public partial class ProjectSelector : Window {
        public delegate void ProjectSelectedDelegate(ProjectSelectedArgs args);
        public ProjectSelectedDelegate OnProjectSelected;

        public ProjectSelector() {
            InitializeComponent();

            SubTitle.Text = $"Clove Engine - {Membrane.Application.getProjectVersion()}";
        }

        private void NewProjectButtonClick(object sender, RoutedEventArgs e) {
            var dialog = new SaveFileDialog();
            dialog.Filter = "Clove project files (*.clvproj)|*.clvproj";
            if (dialog.ShowDialog() == true) {
                OnProjectSelected?.Invoke(new ProjectSelectedArgs() {
                    ProjectPath = Path.GetDirectoryName(dialog.FileName),
                    ProjectName = Path.GetFileNameWithoutExtension(dialog.SafeFileName),
                    FullPath = dialog.FileName,
                    IsNewProject = true
                });
            }
        }

        private void OpenProjectButtonClick(object sender, RoutedEventArgs e) {
            var dialog = new OpenFileDialog();
            dialog.Filter = "Clove project files (*.clvproj)|*.clvproj";
            if (dialog.ShowDialog() == true) {
                OnProjectSelected?.Invoke(new ProjectSelectedArgs() {
                    ProjectPath = Path.GetDirectoryName(dialog.FileName),
                    ProjectName = Path.GetFileNameWithoutExtension(dialog.SafeFileName),
                    FullPath = dialog.FileName,
                    IsNewProject = false
                });
            }
        }
    }
}
