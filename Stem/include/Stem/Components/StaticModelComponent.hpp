#pragma once

#include <Bulb/ECS/Component.hpp>
#include <Bulb/Rendering/Renderables/StaticModel.hpp>

namespace garlic::inline stem {
    class StaticModelComponent : public blb::ecs::Component<StaticModelComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        blb::rnd::StaticModel model;

        //FUNCTIONS
    public:
        StaticModelComponent() = delete;
        StaticModelComponent(blb::rnd::StaticModel model);

        StaticModelComponent(StaticModelComponent const &other);
        StaticModelComponent(StaticModelComponent &&other) noexcept;

        StaticModelComponent &operator=(StaticModelComponent const &other);
        StaticModelComponent &operator=(StaticModelComponent &&other) noexcept;

        ~StaticModelComponent();

        inline blb::rnd::StaticModel &getModel();
    };
}

#include "StaticModelComponent.inl"