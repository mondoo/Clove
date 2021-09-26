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

            //Project file
            using (FileStream projectStream = File.Create(ProjectPath + $"/{ProjectName}.clvproj")) {
                WriteLine(projectStream, "type: yaml");
                WriteLine(projectStream, "version: 1");
                WriteLine(projectStream, $"name: {editorName}");
                WriteLine(projectStream, $"library: {ProjectPath}/lib/Debug/{editorName}.dll");
            }

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

                WriteLine(cmakeStream, "target_link_libraries(");
                WriteLine(cmakeStream, "\t" + editorName);
                WriteLine(cmakeStream, "");
                WriteLine(cmakeStream, "\tPRIVATE");
                WriteLine(cmakeStream, "\t\tClove");
                WriteLine(cmakeStream, ")");
                WriteLine(cmakeStream, "");

                WriteLine(cmakeStream, "set_target_properties(");
                WriteLine(cmakeStream, "\t" + editorName);
                WriteLine(cmakeStream, "\tPROPERTIES");
                WriteLine(cmakeStream, "\t\tARCHIVE_OUTPUT_DIRECTORY \"${CMAKE_CURRENT_SOURCE_DIR}/lib\"");
                WriteLine(cmakeStream, "\t\tLIBRARY_OUTPUT_DIRECTORY \"${CMAKE_CURRENT_SOURCE_DIR}/lib\"");
                WriteLine(cmakeStream, "\t\tRUNTIME_OUTPUT_DIRECTORY \"${CMAKE_CURRENT_SOURCE_DIR}/bin\"");
                WriteLine(cmakeStream, ")");

                //TEMP: Provide membrane with the binary dit - this needs to be removed. Membrane should be able to build and load any valid game dll. Look into installing through cmake and having this cmake list completely standalone
                WriteLine(cmakeStream, "target_compile_definitions(");
                WriteLine(cmakeStream, "\t" + "BulbMembrane");
                WriteLine(cmakeStream, "");
                WriteLine(cmakeStream, "\tPRIVATE");
                WriteLine(cmakeStream, $"\t\tBINARY_DIR=\"${{CMAKE_BINARY_DIR}}\"");
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