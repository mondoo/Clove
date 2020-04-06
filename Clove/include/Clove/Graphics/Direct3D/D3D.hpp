#pragma once

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::gfx::d3d{
	std::shared_ptr<GraphicsFactory> initialiseD3D();
}