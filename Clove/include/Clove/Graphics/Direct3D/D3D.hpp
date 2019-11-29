#pragma once

namespace clv::gfx{
	class RenderDevice;
	class RenderFactory;
}

namespace clv::gfx::d3d{
	std::pair<std::unique_ptr<RenderDevice>, std::unique_ptr<RenderFactory>> initialiseD3D();
}