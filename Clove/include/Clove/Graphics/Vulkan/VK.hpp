#pragma once

namespace clv::gfx {
	class GraphicsFactory;
}

namespace clv::gfx::vk {
	std::shared_ptr<GraphicsFactory> initialiseVK();
}