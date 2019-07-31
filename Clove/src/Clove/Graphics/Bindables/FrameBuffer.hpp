#pragma once

#include "Clove/Graphics/Bindable.hpp"

namespace clv::gfx{
	enum class AttachmentType{
		Colour,
		Depth,
		Stencil,
		Depth_Stencil
	};

	class Texture;

	class FrameBuffer : public Bindable{
		//FUNCTIONS
	public:
		FrameBuffer();
		FrameBuffer(const FrameBuffer& other) = delete;
		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(const FrameBuffer& other) = delete;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;
		virtual ~FrameBuffer();

		virtual void attachTexture(Texture& Texture, AttachmentType attachmentType) = 0;

		virtual bool isComplete() const = 0;
	};
}
