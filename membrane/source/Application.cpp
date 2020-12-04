#include "Membrane/Application.hpp"

#include "Membrane/EditorLayer.hpp"
#include "Membrane/RuntimeLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Audio/Audio.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Graphics/GraphicsBuffer.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/Rendering/GraphicsImageRenderTarget.hpp>
#include <Clove/Systems/RenderSystem.hpp>
#include <Clove/ECS/World.hpp>

namespace garlic::membrane {
    class ConsoleLogger : public clove::Logger::Output {
    public:
        void doLog(clove::LogLevel level, std::string_view msg) override {
            //Conver to std::string as there seems to be issues when using std::data(msg)
            System::Console::WriteLine(gcnew System::String(std::string{ std::begin(msg), std::end(msg) }.c_str()));
        }
    };
}

namespace garlic::membrane {
    Application::Application(int const width, int const height)
        : width{ width }
        , height{ height } {
        using namespace garlic::clove;

        //Set the logger for Clove to redirect to System.Console
        Logger::setOutput(std::make_unique<ConsoleLogger>());

        //Hard coding format to B8G8R8A8_SRGB
        GraphicsImage::Descriptor renderTargetImageDescriptor{};
        renderTargetImageDescriptor.type        = GraphicsImage::Type::_2D;
        renderTargetImageDescriptor.usageFlags  = GraphicsImage::UsageMode::ColourAttachment | GraphicsImage::UsageMode::TransferSource;
        renderTargetImageDescriptor.dimensions  = { width, height };
        renderTargetImageDescriptor.format      = GraphicsImage::Format::B8G8R8A8_SRGB;
        renderTargetImageDescriptor.sharingMode = SharingMode::Concurrent;

        //Use pair as there seems to be an issue when using structured bindings
        auto pair{ clove::Application::createHeadless(GraphicsApi::Vulkan, AudioApi::OpenAl, std::move(renderTargetImageDescriptor)) };
        app          = pair.first.release();
        renderTarget = pair.second;

        //Add systems
        app->getECSWorld()->addSystem<clove::RenderSystem>();

        editorLayer  = new std::shared_ptr<EditorLayer>();
        *editorLayer = std::make_shared<EditorLayer>(clove::vec2ui{ width, height });

        runtimeLayer  = new std::shared_ptr<RuntimeLayer>();
        *runtimeLayer = std::make_shared<RuntimeLayer>();

        app->pushLayer(*runtimeLayer);
        app->pushOverlay(*editorLayer);
    }

    Application::~Application() {
        this->!Application();
    }

    Application::!Application() {
        delete app;
        delete editorLayer;
        delete runtimeLayer;
    }

    bool Application::isRunning() {
        return app->getState() != clove::Application::State::Stopped;
    }

    void Application::tick() {
        app->tick();
    }

    void Application::render(System::IntPtr backBuffer) {
        auto const renderTargetBuffer{ renderTarget->getNextReadyBuffer() };
        size_t constexpr bbp{ 4 };
        renderTargetBuffer->read(backBuffer.ToPointer(), 0, width * height * bbp);
    }

    void Application::shutdown() {
        app->shutdown();
    }

    void Application::resize(int width, int height) {
        renderTarget->resize({ width, height });
        (*editorLayer)->resizeViewport({ width, height });

        this->width  = width;
        this->height = height;
    }

    void Application::addEntity() {
        (*runtimeLayer)->addEntity();
    }
}