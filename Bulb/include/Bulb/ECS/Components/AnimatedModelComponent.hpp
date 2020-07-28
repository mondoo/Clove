#pragma once

#include "Bulb/ECS/Component.hpp"
#include "Bulb/Rendering/Renderables/AnimatedModel.hpp"

namespace blb::ecs {
    class AnimedModelComponent : public Component<AnimedModelComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        rnd::AnimatedModel model;

        //FUNCTIONS
    public:
        AnimedModelComponent() = delete;
        AnimedModelComponent(rnd::AnimatedModel model);

        AnimedModelComponent(const AnimedModelComponent& other);
        AnimedModelComponent(AnimedModelComponent&& other) noexcept;

        AnimedModelComponent& operator=(const AnimedModelComponent& other);
        AnimedModelComponent& operator=(AnimedModelComponent&& other) noexcept;

        ~AnimedModelComponent();

        rnd::AnimatedModel& getModel();
    };
}