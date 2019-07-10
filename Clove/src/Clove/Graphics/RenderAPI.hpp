#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class RenderAPI{
		//VARIABLES
	private:
		static API api;

		//FUNCTIONS
	public:
		RenderAPI() = delete;
		RenderAPI(const RenderAPI& other) = delete;
		RenderAPI(RenderAPI&& other) noexcept;
		RenderAPI& operator=(const RenderAPI& other) = delete;
		RenderAPI& operator=(RenderAPI&& other) noexcept;
		virtual ~RenderAPI();

		RenderAPI(API apiType);

		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;

		static API getAPIType();
	};
}