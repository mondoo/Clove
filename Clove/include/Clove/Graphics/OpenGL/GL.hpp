#pragma once

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::gfx::ogl{
	std::shared_ptr<GraphicsFactory> initialiseOGL();
}