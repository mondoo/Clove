using System.IO;
using System.Text;

namespace Bulb {
    /// <summary>
    /// Generates a new project structure
    /// </summary>
    public class ProjectCreator {
        public string ProjectPath;
        public string ProjectName;

        public void Create() {
            Directory.CreateDirectory(ProjectPath);

            using (FileStream cmakeStream = File.Create(ProjectPath + "/" + "CMakeLists.txt")) {
                byte[] text = new UTF8Encoding(true).GetBytes("#TODO");
                cmakeStream.Write(text, 0, text.Length);
            }
        }
    }
}