#pragma once

namespace clv::gfx{
	enum class TextureStyle{
		Default,
		Cubemap
	};

	enum class TextureUsage{
		Default,
		RenderTarget_Colour,
		RenderTarget_Depth,
		Font
	};

	struct TextureDescriptor{
		TextureStyle style = TextureStyle::Default;
		TextureUsage usage = TextureUsage::Default;
		math::Vector<2, uint32, math::qualifier::defaultp> dimensions = { 0, 0 };
		uint8 arraySize = 1;
	};
}

namespace clv::gfx{
	class Texture{
		//FUNCTIONS
	public:
		virtual ~Texture() = default;

		virtual const TextureDescriptor& getDescriptor() const = 0;
	};
}
