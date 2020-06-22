#pragma once

#include <Bulb/ECS/Entity.hpp>
#include <Bulb/ECS/World.hpp>
#include <Bulb/Layer.hpp>

namespace clv::plt {
    class Window;
}

namespace blb::rnd {
    class Renderer3D;
}

class ExampleLayer : public blb::Layer {
    //VARIABLES
private:
    //The window we want to render to
    std::shared_ptr<clv::plt::Window> window;

    //The renderer we'll use
    std::shared_ptr<blb::rnd::Renderer3D> renderer;

    //The ECS world that this layer has
    blb::ecs::World world;

    //The entities we want in our world
    blb::ecs::Entity cubeEntity;
    blb::ecs::Entity lightEntity;
    blb::ecs::Entity cameraEntity;

    //FUNCTIONS
public:
    ExampleLayer(std::shared_ptr<clv::plt::Window> window);

    //Called when the layer is attached
    virtual void onAttach() override;

    //Called once per frame
    virtual void onUpdate(clv::utl::DeltaTime deltaTime) override;

    //Called when the layer has been detached
    virtual void onDetach() override;
};