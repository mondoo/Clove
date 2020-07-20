#pragma once

#include "Bulb/Rendering/Renderables/StaticModel.hpp"
#include "Bulb/Rendering/Renderables/AnimatedModel.hpp"

namespace clv::gfx{
	class GraphicsFactory;
}

namespace blb::ModelLoader {
    rnd::StaticModel loadStaticModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory);
    rnd::AnimatedModel loadAnimatedModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory);
}