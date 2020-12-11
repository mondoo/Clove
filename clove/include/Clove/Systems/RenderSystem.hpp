#pragma once

#include <Clove/DeltaTime.hpp>

namespace garlic::clove {
    class ForwardRenderer3D;
    class World;
}

namespace garlic::clove {
    class RenderSystem {
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

        void update(World &world, DeltaTime deltaTime);
    };
}
