#pragma once

#include "Tunic/Rendering/Renderables/Model.hpp"

namespace clv::gfx{
	class GraphicsFactory;
}

namespace tnc::ModelLoader {
	rnd::Model loadModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory);
}