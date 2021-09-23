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

            string sourceName = $"{ProjectName}Source";
            string editorName = $"{ProjectName}Editor";

            using (FileStream cmakeStream = File.Create(ProjectPath + "/" + "CMakeLists.txt")) {
                //Project Source 
                WriteLine(cmakeStream, "set(");
                WriteLine(cmakeStream, "\t" + sourceName);
                WriteLine(cmakeStream, "\t#ADD SOURCE HERE");
                WriteLine(cmakeStream, ")");
                WriteLine(cmakeStream, "");

                //Game executable
                //TODO

                //Editor library
                WriteLine(cmakeStream, "add_library(");
                WriteLine(cmakeStream, "\t" + editorName + " MODULE");
                WriteLine(cmakeStream, $"\t\t${{{sourceName}}}");
                WriteLine(cmakeStream, ")");
                WriteLine(cmakeStream, "");

                //Link Libraries
                WriteLine(cmakeStream, "target_link_libraries(");
                WriteLine(cmakeStream, "\t" + editorName);
                WriteLine(cmakeStream, "");
                WriteLine(cmakeStream, "\tPRIVATE");
                WriteLine(cmakeStream, "\t\tClove");
                WriteLine(cmakeStream, ")");
            }
        }

        private static void WriteLine(FileStream fs, string value) {
            byte[] info = new UTF8Encoding(true).GetBytes(value + "\n");
            fs.Write(info, 0, info.Length);
        }
    }
}