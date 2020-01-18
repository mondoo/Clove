#pragma once

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::gfx::mtl{
	std::unique_ptr<GraphicsFactory> initialiseMTL();
}
