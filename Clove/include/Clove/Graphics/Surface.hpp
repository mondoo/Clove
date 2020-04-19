#pragma once

#include "Clove/Graphics/FactoryChild.hpp"

namespace clv::gfx {
	class RenderTarget;
}

namespace clv::gfx {
	class Surface : public FactoryChild {
		//FUNCTIONS
	public:
		virtual ~Surface() = default;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVsync() const = 0;

		virtual void resizeBuffers(const mth::vec2ui& size) = 0;

		virtual void makeCurrent() = 0;

		virtual void present() = 0;

		virtual std::shared_ptr<RenderTarget> getRenderTarget() const = 0;
	};
}