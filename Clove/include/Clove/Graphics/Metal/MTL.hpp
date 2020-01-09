#pragma once

namespace clv::gfx{
	class RenderFactory;
}

namespace clv::gfx::mtl{
	std::unique_ptr<RenderFactory> initialiseMTL();
}
