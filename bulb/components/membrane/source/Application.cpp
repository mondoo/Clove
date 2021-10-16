#include "Membrane/Application.hpp"

#include "Membrane/EditorSubSystem.hpp"
#include "Membrane/EditorViewport.hpp"
#include "Membrane/MessageHandler.hpp"
#include "Membrane/Messages.hpp"
#include "Membrane/RuntimeSubSystem.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Graphics/GhaBuffer.hpp>
#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/Rendering/GraphicsImageRenderTarget.hpp>
#include <Clove/Serialisation/Node.hpp>
#include <Clove/Serialisation/Yaml.hpp>
#include <filesystem>
#include <msclr/marshal_cppstd.h>

CLOVE_DECLARE_LOG_CATEGORY(Membrane)

#ifndef GAME_OUTPUT_DIR
    #error "GAME_OUTPUT_DIR is not defined. Please define this for BulbMembrane and point it to build output location."
#endif
#ifndef GAME_NAME
    #error "GAME_NAME is not defined. Please define this for BulbMembrane to provide the name of the target to build."
#endif
#ifndef GAME_MODULE_DIR
    #error "GAME_MODULE_DIR is not defined. Please define this for BulbMembrane and point it to the game dll to load."
#endif
#ifndef GAME_DIR
    #error "GAME_DIR is not defined. Please define this for BulbMembrane and point it to the root game folder."
#endif

typedef void (*setUpEditorApplicationFn)(clove::Application *app);
typedef void (*tearDownEditorApplicationFn)(clove::Application *app);

namespace {
    std::string_view constexpr dllPath{ GAME_MODULE_DIR "/" GAME_NAME ".dll" };
}

namespace membrane {
    static std::filesystem::path const cachedProjectsPath{ "projects.yaml" };

    Application::Application(int const width, int const height)
        : width{ width }
        , height{ height } {
        using namespace clove;

        GhaImage::Descriptor renderTargetImageDescriptor{};
        renderTargetImageDescriptor.type        = GhaImage::Type::_2D;
        renderTargetImageDescriptor.usageFlags  = GhaImage::UsageMode::ColourAttachment | GhaImage::UsageMode::TransferSource;
        renderTargetImageDescriptor.dimensions  = { width, height };
        renderTargetImageDescriptor.format      = GhaImage::Format::B8G8R8A8_SRGB;//Hard coding format to B8G8R8A8_SRGB as that is what the WriteableBitmap is set to
        renderTargetImageDescriptor.sharingMode = SharingMode::Concurrent;

        viewport = gcnew EditorViewport{};

        //Use pair as there seems to be an issue when using structured bindings
        auto pair{ clove::Application::createHeadless(GraphicsApi::Vulkan, AudioApi::OpenAl, std::move(renderTargetImageDescriptor), viewport->getKeyboard(), viewport->getMouse()) };
        app          = pair.first.release();
        renderTarget = pair.second;

        auto *vfs{ app->getFileSystem() };
        vfs->mount(GAME_DIR "/content", ".");
        std::filesystem::create_directories(vfs->resolve("."));

        app->pushSubSystem<EditorSubSystem>();

        MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_Stop ^>(this, &Application::setEditorMode));
        MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_Play ^>(this, &Application::setRuntimeMode));
    }

    Application::~Application() {
        this->!Application();
    }

    Application::!Application() {
        if(gameLibrary != nullptr) {
            FreeLibrary(gameLibrary);
        }
        delete app;
    }

    void Application::loadGameDll() {
        std::optional<std::filesystem::path> fallbackDll{};
        if(gameLibrary != nullptr) {
            CLOVE_LOG(Membrane, clove::LogLevel::Trace, "Unloading {0} to prepare for compilation and reload", GAME_NAME);

            if(tearDownEditorApplicationFn tearDownEditorApplication{ (tearDownEditorApplicationFn)GetProcAddress(gameLibrary, "tearDownEditorApplication") }; tearDownEditorApplication != nullptr) {
                (tearDownEditorApplication)(app);
            } else {
                CLOVE_LOG(Membrane, clove::LogLevel::Error, "Could not load game tear down function. Please provide 'tearDownEditorApplication' in client code.");
                return;
            }

            FreeLibrary(gameLibrary);

            try {
                fallbackDll = GAME_MODULE_DIR "/" GAME_NAME "_copy.dll";
                std::filesystem::copy_file(dllPath, fallbackDll.value(), std::filesystem::copy_options::overwrite_existing);
            } catch(std::exception e) {
                CLOVE_LOG(Membrane, clove::LogLevel::Error, "{0}", e.what());
                return;
            }
        }

        std::filesystem::remove(dllPath);

        CLOVE_LOG(Membrane, clove::LogLevel::Debug, "Configuring and compiling {0}", GAME_NAME);

        {
            std::stringstream configureStream{};
            configureStream << "cmake -G \"Visual Studio 16 2019\" " << ROOT_DIR;
            std::system(configureStream.str().c_str());
        }

        {
            std::stringstream buildStream{};
            buildStream << "cmake --build " << GAME_OUTPUT_DIR << " --target " << GAME_NAME << " --config Debug";
            std::system(buildStream.str().c_str());
        }

        bool const loadingSuccessful{ tryLoadGameDll(dllPath) };

        if(!loadingSuccessful) {
            if(fallbackDll.has_value()) {
                CLOVE_LOG(Membrane, clove::LogLevel::Debug, "Attempting to load fallback Dll...");

                if(!tryLoadGameDll(fallbackDll->string())) {
                    throw gcnew System::Exception{ "Loading of backup game Dll failed." };
                }
            } else {
                throw gcnew System::Exception{ "Loading of game Dll failed." };
            }
        }

        CLOVE_LOG(Membrane, clove::LogLevel::Info, "Successfully loaded {0} dll", GAME_NAME);
    }

    bool Application::isRunning() {
        return app->getState() != clove::Application::State::Stopped;
    }

    void Application::tick() {
        viewport->processInput();
        app->tick();
    }

    void Application::render(System::IntPtr backBuffer) {
        auto const renderTargetBuffer{ renderTarget->getNextReadyBuffer() };
        size_t constexpr bbp{ 4 };
        renderTargetBuffer->read(backBuffer.ToPointer(), 0, width * height * bbp);
    }

    void Application::shutdown() {
        if(isInEditorMode) {
            app->getSubSystem<EditorSubSystem>().saveScene();
        }
        app->shutdown();
    }

    void Application::resize(int width, int height) {
        renderTarget->resize({ width, height });

        this->width  = width;
        this->height = height;
    }

    System::String ^ Application::resolveVfsPath(System::String ^ path) {
        System::String ^ managedPath { path };
        std::string unManagedPath{ msclr::interop::marshal_as<std::string>(managedPath) };
        return gcnew System::String(app->getFileSystem()->resolve(unManagedPath).c_str());
    }

    System::String ^ Application::getProjectVersion() {
        return gcnew System::String{ CLOVE_VERSION };
    }

    void Application::setEditorMode(Editor_Stop ^ message) {
        app->popSubSystem<RuntimeSubSystem>();
        app->pushSubSystem<EditorSubSystem>();
        isInEditorMode = true;
    }

    void Application::setRuntimeMode(Editor_Play ^ message) {
        app->getSubSystem<EditorSubSystem>().saveScene();

        app->popSubSystem<EditorSubSystem>();
        app->pushSubSystem<RuntimeSubSystem>();
        isInEditorMode = false;
    }

    bool Application::tryLoadGameDll(std::string_view path) {
        if(gameLibrary = LoadLibrary(path.data()); gameLibrary != nullptr) {
            if(setUpEditorApplicationFn setUpEditorApplication{ (setUpEditorApplicationFn)GetProcAddress(gameLibrary, "setUpEditorApplication") }; setUpEditorApplication != nullptr) {
                (setUpEditorApplication)(app);
            } else {
                CLOVE_LOG(Membrane, clove::LogLevel::Error, "Could not load game initialise function. Please provide 'setUpEditorApplication' in client code.");
                return false;
            }
        } else {
            CLOVE_LOG(Membrane, clove::LogLevel::Error, "Could not load game dll. File does not exist");
            return false;
        }

        return true;
    }
}