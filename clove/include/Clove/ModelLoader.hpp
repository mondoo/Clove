#pragma once

#include "Clove/Rendering/Renderables/AnimatedModel.hpp"
#include "Clove/Rendering/Renderables/StaticModel.hpp"

#include <string_view>

namespace garlic::clove {
    class GhaFactory;
}

namespace garlic::clove::ModelLoader {
    /**
     * @brief Loads a StaticModel from a given file path.
     * @param modelFilePath File path to a model.
     * @param graphicsFactory A pointer to a GhaFactory. Required to create the internal buffers of the model
     * @returns A StaticModel.
     */
    StaticModel loadStaticModel(std::string_view modelFilePath);
    /**
     * @brief Loads an AnimatedModel from a given file path.
     * @details An AnimatedModel contains a Skeleton and Animator to generate it's position for a given frame.
     * @param modelFilePath File path to a model.
     * @param graphicsFactory A pointer to a GhaFactory. Required to create the internal buffers of the model
     * @returns An AnimatedModel.
     */
    AnimatedModel loadAnimatedModel(std::string_view modelFilePath);
}