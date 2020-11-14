#pragma once

#include "Bulb/ECS/System.hpp"

namespace clv::plt {
    class Window;
}

namespace blb::rnd {
    class ForwardRenderer3D;
}

namespace blb::ecs {
    class RenderSystem : public System {
        //VARIABLES
    private:
        std::shared_ptr<rnd::ForwardRenderer3D> renderer;

        //FUNCTIONS
    public:
        RenderSystem() = delete;
        RenderSystem(std::shared_ptr<rnd::ForwardRenderer3D> renderer);

        RenderSystem(RenderSystem const &other) = delete;
        RenderSystem(RenderSystem &&other) noexcept;

        RenderSystem &operator=(RenderSystem const &other) = delete;
        RenderSystem &operator=(RenderSystem &&other) noexcept;

        ~RenderSystem();

        void update(World &world, clv::utl::DeltaTime deltaTime) override;
    };
}
