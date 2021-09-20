using Microsoft.Win32;
using System.IO;
using System.Windows;

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
        }

        private void NewProjectButtonClick(object sender, RoutedEventArgs e) {
            var dialog = new SaveFileDialog();
            dialog.Filter = "Clove project files (*.clvproj)|*.clvproj";
            if (dialog.ShowDialog() == true) {
                File.Create(dialog.FileName);
                FileInfo info = new FileInfo(dialog.FileName);

                OnProjectSelected?.Invoke(new ProjectSelectedArgs() {
                    ProjectPath = info.DirectoryName,
                    ProjectName = Path.GetFileNameWithoutExtension(info.Name),
                    FullPath = dialog.FileName,
                    IsNewProject = true
                });
            }
        }

        private void OpenProjectButtonClick(object sender, RoutedEventArgs e) {
            var dialog = new OpenFileDialog();
            dialog.Filter = "Clove project files (*.clvproj)|*.clvproj";
            if (dialog.ShowDialog() == true) {
                FileInfo info = new FileInfo(dialog.FileName);

                OnProjectSelected?.Invoke(new ProjectSelectedArgs() {
                    ProjectPath = info.DirectoryName,
                    ProjectName = Path.GetFileNameWithoutExtension(info.Name),
                    FullPath = dialog.FileName,
                    IsNewProject = false
                });
            }
        }
    }
}
