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
            Directory.CreateDirectory(ProjectPath + "/source");

            string sourceName = $"{ProjectName}Source";
            string editorName = $"{ProjectName}Editor";

            //TODO: Create *.clvproj file

            //Cmake lists
            using (FileStream cmakeStream = File.Create(ProjectPath + "/CMakeLists.txt")) {
                //Project Source 
                WriteLine(cmakeStream, "set(");
                WriteLine(cmakeStream, "\t" + sourceName);
                WriteLine(cmakeStream, "\t#Generated");
                WriteLine(cmakeStream, $"\tsource/{editorName}.cpp");
                WriteLine(cmakeStream, $"\t#{ProjectName} source");
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
                WriteLine(cmakeStream, "");

                //Definitions
                WriteLine(cmakeStream, "target_compile_definitions(");
                WriteLine(cmakeStream, "\t" + "BulbMembrane");
                WriteLine(cmakeStream, "");
                WriteLine(cmakeStream, "\tPRIVATE");
                WriteLine(cmakeStream, $"\t\tGAME_MODULE=\"${{CMAKE_CURRENT_BINARY_DIR}}/Debug/{editorName}.dll\""); //TODO: Remove 'debug'
                WriteLine(cmakeStream, ")");
            }

            //Source files
            using (FileStream sourceStream = File.Create(ProjectPath + $"/source/{editorName}.cpp")) {
                WriteLine(sourceStream, "#include <Clove/Application.hpp>");
                WriteLine(sourceStream, "#include <Clove/SubSystems/AudioSubSystem.hpp>");
                WriteLine(sourceStream, "#include <Clove/SubSystems/PhysicsSubSystem.hpp>");
                WriteLine(sourceStream, "#include <Clove/SubSystems/RenderSubSystem.hpp>");
                WriteLine(sourceStream, "#include <Clove/SubSystems/TransformSubSystem.hpp>");
                WriteLine(sourceStream, "#include <memory>");
                WriteLine(sourceStream, "");

                WriteLine(sourceStream, "extern \"C\" {");
                WriteLine(sourceStream, "__declspec(dllexport) void setUpEditorApplication(clove::Application *app) {");
                WriteLine(sourceStream, "\tclove::Application::set(app);");
                WriteLine(sourceStream, "");
                WriteLine(sourceStream, "\t//Engine default subsystems");
                WriteLine(sourceStream, "\tapp->pushSubSystem<clove::TransformSubSystem>(clove::Application::SubSystemGroup::Initialisation, app->getEntityManager());");
                WriteLine(sourceStream, "\tapp->pushSubSystem<clove::PhysicsSubSystem>(clove::Application::SubSystemGroup::Initialisation, app->getEntityManager());");
                WriteLine(sourceStream, "\tapp->pushSubSystem<clove::AudioSubSystem>(clove::Application::SubSystemGroup::Render, app->getEntityManager());");
                WriteLine(sourceStream, "\tapp->pushSubSystem<clove::RenderSubSystem>(clove::Application::SubSystemGroup::Render, app->getRenderer(), app->getEntityManager());");
                WriteLine(sourceStream, "");
                WriteLine(sourceStream, "\t//User subsystems");
                WriteLine(sourceStream, "\t//...");
                WriteLine(sourceStream, "}");
                WriteLine(sourceStream, "}");
            }
        }

        private static void WriteLine(FileStream fs, string value) {
            byte[] info = new UTF8Encoding(true).GetBytes(value + "\n");
            fs.Write(info, 0, info.Length);
        }
    }
}