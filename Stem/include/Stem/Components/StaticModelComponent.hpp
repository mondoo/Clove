#pragma once

#include "Stem/Rendering/Renderables/StaticModel.hpp"

#include <Bulb/ECS/Component.hpp>

namespace garlic::inline stem {
    class StaticModelComponent : public blb::ecs::Component<StaticModelComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        StaticModel model;

        //FUNCTIONS
    public:
        StaticModelComponent() = delete;
        StaticModelComponent(StaticModel model);

        StaticModelComponent(StaticModelComponent const &other);
        StaticModelComponent(StaticModelComponent &&other) noexcept;

        StaticModelComponent &operator=(StaticModelComponent const &other);
        StaticModelComponent &operator=(StaticModelComponent &&other) noexcept;

        ~StaticModelComponent();

        inline StaticModel &getModel();
    };
}

#include "StaticModelComponent.inl"