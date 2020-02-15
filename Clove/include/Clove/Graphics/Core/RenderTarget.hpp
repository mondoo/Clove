#pragma once

namespace clv::gfx{
	class RenderTarget{
		//FUNCTIONS
	public:
		virtual ~RenderTarget() = default;

		virtual void clear() = 0;

		virtual void setClearColour(const mth::vec4f& colour) = 0;
	};
}
