#pragma once

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::gfx::mtl{
	std::shared_ptr<GraphicsFactory> initialiseMTL();
}
