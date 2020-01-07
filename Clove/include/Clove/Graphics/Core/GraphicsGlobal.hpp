#pragma once

#include "Clove/Graphics/Core/RenderDevice.hpp"
#include "Clove/Graphics/Core/RenderFactory.hpp"

namespace clv::gfx::global{
	inline std::unique_ptr<RenderDevice> graphicsDevice;
	inline std::unique_ptr<RenderFactory> graphicsFactory;

	void initialise(gfx::API api);
}