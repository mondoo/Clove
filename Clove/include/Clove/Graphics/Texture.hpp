#pragma once

#include "Clove/Graphics/FactoryChild.hpp"

namespace clv::gfx {
	enum class TextureStyle {
		Default,
		Cubemap
	};

	enum class TextureUsage {
		Default,
		RenderTarget_Colour,
		RenderTarget_Depth,
		Font
	};

	enum class TextureFilter {
		Nearest,
		Linear,
	};

	struct TextureDescriptor {
		TextureStyle style = TextureStyle::Default;
		TextureUsage usage = TextureUsage::Default;
		TextureFilter filtering = TextureFilter::Linear;
		mth::vec<2, uint32_t, mth::qualifier::defaultp> dimensions = { 0, 0 };
		uint8_t arraySize = 1;
	};
}

namespace clv::gfx {
	class Texture : public FactoryChild {
		//FUNCTIONS
	public:
		virtual ~Texture() = default;

		virtual const TextureDescriptor& getDescriptor() const = 0;
	};
}
