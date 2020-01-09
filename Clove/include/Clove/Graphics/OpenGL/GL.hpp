#pragma once

namespace clv::gfx{
	class RenderFactory;
}

namespace clv::gfx::ogl{
	std::unique_ptr<RenderFactory> initialiseOGL();
}