#pragma once

#include "Clove/Rendering/Renderables/StaticModel.hpp"

#include <Clove/ECS/Component.hpp>

namespace garlic::clove {
    class StaticModelComponent : public Component<StaticModelComponent> {
        friend class RenderLayer;

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