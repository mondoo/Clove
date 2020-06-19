#include <Bulb/Bulb.hpp>

int main() {
    //Create the platform instance we'll use to make OS calls
    std::unique_ptr<clv::plt::Platform> platformInstance = clv::plt::createPlatformInstance();

    //Create the window which we will render to
    std::shared_ptr<clv::plt::Window> window = platformInstance->createWindow({ "Skeletal Animation", 1280, 720, clv::plt::getPlatformPreferedAPI() });
    window->setVSync(true);

    //Cache of the 'previous frame time' that'll be used to calculate the delta time
    auto prevFrameTime = std::chrono::system_clock::now();

    auto model    = blb::ModelLoader::loadModel(SOURCE_DIR "/CharacterRunning.fbx", window->getGraphicsFactory());
    auto renderer = std::make_shared<blb::rnd::Renderer3D>(*window);
    auto camera   = blb::rnd::Camera{ *window, blb::rnd::ProjectionMode::Perspective };

    static constexpr float size     = 50.0f;
    static constexpr float nearDist = 0.5f;
    static constexpr float farDist  = 1000.0f;

    blb::rnd::DirectionalLightData dirLightData;
    dirLightData.direction = { 0.0f, -1.0f, 1.0f };
    dirLightData.ambient   = { 0.01f, 0.01f, 0.01f };
    dirLightData.diffuse   = { 0.75f, 0.75f, 0.75f };
    dirLightData.specular  = { 1.0f, 1.0f, 1.0f };

    blb::rnd::DirectionalLight dirLight;
    dirLight.data            = std::move(dirLightData);
    dirLight.shadowTransform = clv::mth::createOrthographicMatrix(-size, size, -size, size, nearDist, farDist);

    //Run our program while the window is open
    while(window->isOpen()) {
        //Calculate the delta time
        auto currFrameTime                        = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
        prevFrameTime                             = currFrameTime;

        //Tell our window we're beginning a frame so it can handle any input events
        window->beginFrame();

        renderer->begin();

        //Camera
        const clv::mth::vec3f position{ 0.0f, 5.0f, -10.0f };
        const clv::mth::vec3f camFront{ 0.0f, 0.0f, 1.0f };
        const clv::mth::vec3f camUp{ 0.0f, 1.0f, 0.0f };
        const clv::mth::mat4f lookAt = clv::mth::lookAt(position, position + camFront, camUp);
        camera.setView(lookAt);
        blb::rnd::CameraRenderData renderData{};
        renderData.lookAt     = lookAt;
        renderData.position   = position;
        renderData.projection = camera.getProjection();
        renderer->submitCamera({ camera.getViewport(), std::move(renderData), camera.getRenderTarget() });

        //Light
        renderer->submitLight(dirLight);

        //Mesh
        for(auto& mesh : model.getMeshes()) {
            auto rot = clv::mth::rotate(clv::mth::mat4f{ 1.0f }, clv::mth::asRadians(-90.0f), { 1.0f, 0.0f, 0.0f });
            rot = clv::mth::rotate(rot, clv::mth::asRadians(90.0f), { 0.0f, 0.0f, 1.0f });

            mesh->getMaterialInstance().setData(clv::gfx::BBP_ModelData, blb::rnd::VertexData{ rot, clv::mth::transpose(clv::mth::inverse(rot)) }, clv::gfx::ShaderStage::Vertex);
            renderer->submitMesh(mesh);
        }

        renderer->end();

        //Tell our window we've finished frame so it can present it's back buffer
        window->endFrame();
    }

    return 0;
}