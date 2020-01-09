#pragma once

namespace clv::gfx{
	class RenderFactory;
}

namespace clv::gfx::d3d{
	std::unique_ptr<RenderFactory> initialiseD3D();
}