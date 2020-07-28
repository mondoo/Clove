#include <Bulb/Bulb.hpp>

int main() {
    //Create the platform instance we'll use to make OS calls
    std::unique_ptr<clv::plt::Platform> platformInstance = clv::plt::createPlatformInstance();

    //Create the window which we will render to
    std::shared_ptr<clv::plt::Window> window = platformInstance->createWindow({ "Skeletal Animation", 1280, 720, clv::plt::getPlatformPreferedAPI() });
    window->setVSync(true);

    //Cache of the 'previous frame time' that'll be used to calculate the delta time
    auto prevFrameTime = std::chrono::system_clock::now();

    //Create the objects we will render
    blb::rnd::AnimatedModel model = blb::ModelLoader::loadAnimatedModel(SOURCE_DIR "/CharacterRunning.fbx", window->getGraphicsFactory());
    blb::rnd::StaticModel floor   = blb::ModelLoader::loadStaticModel(SOURCE_DIR "/cube.obj", window->getGraphicsFactory());

    auto camera   = blb::rnd::Camera{ *window, blb::rnd::ProjectionMode::Perspective };
    auto renderer = std::make_shared<blb::rnd::Renderer3D>(*window);

    //Create the data structures that we'll use to describe a directional light
    constexpr float size     = 50.0f;
    constexpr float nearDist = 0.5f;
    constexpr float farDist  = 1000.0f;

    blb::rnd::DirectionalLightData dirLightData{};
    dirLightData.direction = { 1.0f, -1.0f, 0.0f };
    dirLightData.ambient   = { 0.01f, 0.01f, 0.01f };
    dirLightData.diffuse   = { 0.75f, 0.75f, 0.75f };
    dirLightData.specular  = { 1.0f, 1.0f, 1.0f };

    blb::rnd::DirectionalLight dirLight{};
    dirLight.data            = std::move(dirLightData);
    dirLight.shadowTransform = clv::mth::createOrthographicMatrix(-size, size, -size, size, nearDist, farDist) * clv::mth::lookAt(-clv::mth::normalise(dirLight.data.direction) * (farDist / 2.0f), clv::mth::vec3f{ 0.0f, 0.0f, 0.0f }, clv::mth::vec3f{ 0.0f, 1.0f, 0.0f });

    //Run our program while the window is open
    while(window->isOpen()) {
        //Calculate the delta time
        auto currFrameTime                        = std::chrono::system_clock::now();
        std::chrono::duration<float> deltaSeconds = currFrameTime - prevFrameTime;
        prevFrameTime                             = currFrameTime;

        //Tell our window we're beginning a frame so it can handle any input events
        window->beginFrame();

        renderer->begin();

        //Calculate and then submit our camera to the renderer
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

        renderer->submitLight(dirLight);

        //Calculate a rotation matrix for our model
        static float angle = 0.0f;
        angle += deltaSeconds.count() * 25.0f;
        auto rot = clv::mth::rotate(clv::mth::mat4f{ 1.0f }, clv::mth::asRadians(-90.0f), { 1.0f, 0.0f, 0.0f });
        rot      = clv::mth::rotate(rot, clv::mth::asRadians(90.0f + angle), { 0.0f, 0.0f, 1.0f });

        //Retrieve the matrix palet for the current frame. This contains all the calculated joint positions of our skeleton
        std::array<clv::mth::mat4f, blb::rnd::MAX_JOINTS> matrixPalet = model.update(deltaSeconds.count());
        for(auto& mesh : model.getMeshes()) {
            //Submit the matrix palet and rotation matrix to our mesh's material. This will send the data to the shader
            mesh->getMaterialInstance().setData(clv::gfx::BBP_SkeletalData, matrixPalet.data(), sizeof(clv::mth::mat4f) * blb::rnd::MAX_JOINTS, clv::gfx::ShaderStage::Vertex);
            mesh->getMaterialInstance().setData(clv::gfx::BBP_ModelData, blb::rnd::VertexData{ rot, clv::mth::transpose(clv::mth::inverse(rot)) }, clv::gfx::ShaderStage::Vertex);

            //Tell the renderer we're submitting this as an animated mesh
            renderer->submitAnimatedMesh(mesh);
        }

        //Do something similar for our floor but this won;t animated
        auto translation = clv::mth::translate(clv::mth::mat4f{ 1.0f }, { 0.0f, 0.0f, 0.0f });
        auto scale       = clv::mth::scale(clv::mth::mat4f{ 1.0f }, { 10.0f, 0.1f, 10.0f });
        auto transform = translation * scale;

        for(auto& mesh : floor.getMeshes()) {
            mesh->getMaterialInstance().setData(clv::gfx::BBP_ModelData, blb::rnd::VertexData{ transform, clv::mth::transpose(clv::mth::inverse(transform)) }, clv::gfx::ShaderStage::Vertex);

            renderer->submitStaticMesh(mesh);
        }

        renderer->end();

        //Tell our window we've finished frame so it can present it's back buffer
        window->endFrame();
    }

    return 0;
}