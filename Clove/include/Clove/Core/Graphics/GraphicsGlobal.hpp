#pragma once

#include "Clove/Core/Graphics/RenderDevice.hpp"
#include "Clove/Core/Graphics/RenderFactory.hpp"

namespace clv::gfx::global{
	inline std::unique_ptr<RenderDevice> graphicsDevice;
	inline std::unique_ptr<RenderFactory> graphicsFactory;

	void initialise(gfx::API api);
}