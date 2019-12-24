#pragma once

#include "Clove/Core/Graphics/Surface.hpp"

namespace clv::gfx::mtl{
	class MTLSurface : public Surface{
		//VARIABLES
	private:
		MTKView* view = nullptr;
		
		//FUNCTIONS
	public:
		//TODO: Ctors
		MTLSurface(id<MTLDevice> mtlDevice, void* windowData);
		virtual ~MTLSurface();
		
		virtual void setVSync(bool vsync) override;
		virtual bool isVsync() const override;

		virtual void resizeBuffers(const mth::vec2ui& size) override;

		virtual void present() override;
		
		MTKView* getView() const;
	};
}
