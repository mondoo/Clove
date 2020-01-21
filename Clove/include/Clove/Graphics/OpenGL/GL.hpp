#pragma once

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::gfx::ogl{
	std::unique_ptr<GraphicsFactory> initialiseOGL();
}