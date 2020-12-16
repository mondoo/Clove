#pragma once

#include "Clove/Layer.hpp"

#include <Clove/DeltaTime.hpp>

namespace garlic::clove {
    class ForwardRenderer3D;
    class EntityManager;
}

namespace garlic::clove {
    class RenderLayer : public Layer {
        //VARIABLES
    private:
        ForwardRenderer3D *renderer{ nullptr };
        EntityManager *entityManager{ nullptr };

        //FUNCTIONS
    public:
        RenderLayer();

        RenderLayer(RenderLayer const &other) = delete;
        RenderLayer(RenderLayer &&other) noexcept;

        RenderLayer &operator=(RenderLayer const &other) = delete;
        RenderLayer &operator=(RenderLayer &&other) noexcept;

        ~RenderLayer();

        void onUpdate(DeltaTime const deltaTime) override;
    };
}
