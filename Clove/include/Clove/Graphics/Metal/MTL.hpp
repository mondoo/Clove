#pragma once

namespace clv::gfx{
	class RenderDevice;
	class RenderFactory;
}

namespace clv::gfx::mtl{
	std::pair<std::unique_ptr<RenderDevice>, std::unique_ptr<RenderFactory>> initialiseMTL();
}
