#pragma once

#include "Bulb/Rendering/Renderables/StaticModel.hpp"
#include "Bulb/Rendering/Renderables/AnimatedModel.hpp"

namespace clv::gfx{
	class GraphicsFactory;
}

namespace blb::ModelLoader {
    /**
     * @brief Loads a StaticModel from a given file path.
     * @param modelFilePath File path to a model.
     * @param graphicsFactory A pointer to a GraphicsFactory. Required to create the internal buffers of the model
     * @returns A StaticModel.
     */
    rnd::StaticModel loadStaticModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory);
    /**
     * @brief Loads an AnimatedModel from a given file path.
     * @details An AnimatedModel contains a Skeleton and Animator to generated it's position for a given frame.
     * @param modelFilePath File path to a model.
     * @param graphicsFactory A pointer to a GraphicsFactory. Required to create the internal buffers of the model
     * @returns An AnimatedModel.
     */
    rnd::AnimatedModel loadAnimatedModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory);
}