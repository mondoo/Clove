#pragma once

#include "Clove/Graphics/Bindables/FrameBuffer.hpp"

namespace clv::gfx{
	class GL4FrameBuffer : public FrameBuffer{
		//VARIABLES
	private:
		unsigned int renderID = 0;

		//FUNCTIONS
	public:
		GL4FrameBuffer();
		GL4FrameBuffer(const GL4FrameBuffer& other) = delete;
		GL4FrameBuffer(GL4FrameBuffer&& other) noexcept;
		GL4FrameBuffer& operator=(const GL4FrameBuffer& other) = delete;
		GL4FrameBuffer& operator=(GL4FrameBuffer&& other) noexcept;
		virtual ~GL4FrameBuffer();

		virtual void bind() override;

		virtual void attachTexture(Texture& Texture, AttachmentType attachmentType) override;

		virtual bool isComplete() const override;
	};
}
