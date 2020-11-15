#pragma once

#include "Stem/Rendering/Renderables/AnimatedModel.hpp"

#include <Bulb/ECS/Component.hpp>

namespace garlic::inline stem {
    class AnimatedModelComponent : public blb::ecs::Component<AnimatedModelComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        AnimatedModel model;

        //FUNCTIONS
    public:
        AnimatedModelComponent() = delete;
        AnimatedModelComponent(AnimatedModel model);

        AnimatedModelComponent(AnimatedModelComponent const &other);
        AnimatedModelComponent(AnimatedModelComponent &&other) noexcept;

        AnimatedModelComponent &operator=(AnimatedModelComponent const &other);
        AnimatedModelComponent &operator=(AnimatedModelComponent &&other) noexcept;

        ~AnimatedModelComponent();

        inline AnimatedModel &getModel();
    };
}

#include "AnimatedModelComponent.inl"