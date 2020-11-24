#pragma once

#include <Clove/ECS/System.hpp>

namespace clv::plt {
    class Window;
}

namespace garlic::clove {
    class ForwardRenderer3D;
}

namespace garlic::clove {
    class RenderSystem : public System {
        //VARIABLES
    private:
        ForwardRenderer3D *renderer;

        //FUNCTIONS
    public:
        RenderSystem();

        RenderSystem(RenderSystem const &other) = delete;
        RenderSystem(RenderSystem &&other) noexcept;

        RenderSystem &operator=(RenderSystem const &other) = delete;
        RenderSystem &operator=(RenderSystem &&other) noexcept;

        ~RenderSystem();

        void update(World &world, DeltaTime deltaTime) override;
    };
}
