#pragma once

#include "Clove/Rendering/Renderables/AnimatedModel.hpp"

namespace garlic::clove {
    class AnimatedModelComponent {
        friend class RenderLayer;

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