#pragma once

#include "Clove/SubSystem.hpp"

#include <Clove/DeltaTime.hpp>

namespace clove {
    class ForwardRenderer3D;
    class EntityManager;
}

namespace clove {
    class RenderSubSystem : public SubSystem {
        //VARIABLES
    private:
        ForwardRenderer3D *renderer{ nullptr };
        EntityManager *entityManager{ nullptr };

        //FUNCTIONS
    public:
		RenderSubSystem() = delete;
        RenderSubSystem(ForwardRenderer3D *renderer, EntityManager *entityManager);

        RenderSubSystem(RenderSubSystem const &other) = delete;
        RenderSubSystem(RenderSubSystem &&other) noexcept;

        RenderSubSystem &operator=(RenderSubSystem const &other) = delete;
        RenderSubSystem &operator=(RenderSubSystem &&other) noexcept;

        ~RenderSubSystem();

        void onUpdate(DeltaTime const deltaTime) override;
    };
}
