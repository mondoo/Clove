#pragma once

#include <Bulb/ECS/Component.hpp>
#include <Bulb/Rendering/Renderables/AnimatedModel.hpp>

namespace garlic::inline stem {
    class AnimedModelComponent : public blb::ecs::Component<AnimedModelComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        blb::rnd::AnimatedModel model;

        //FUNCTIONS
    public:
        AnimedModelComponent() = delete;
        AnimedModelComponent(blb::rnd::AnimatedModel model);

        AnimedModelComponent(AnimedModelComponent const &other);
        AnimedModelComponent(AnimedModelComponent &&other) noexcept;

        AnimedModelComponent &operator=(AnimedModelComponent const &other);
        AnimedModelComponent &operator=(AnimedModelComponent &&other) noexcept;

        ~AnimedModelComponent();

        inline blb::rnd::AnimatedModel &getModel();
    };
}

#include "AnimatedModelComponent.inl"