#pragma once

#include "Clove/Graphics/Bindables/FrameBuffer.hpp"

namespace clv::gfx{
	class GL4FrameBuffer : public FrameBuffer{
		//VARIABLES
	private:

		//TODO

		//FUNCTIONS
	public:
		GL4FrameBuffer();
		GL4FrameBuffer(const GL4FrameBuffer& other) = delete;
		GL4FrameBuffer(GL4FrameBuffer&& other) noexcept;
		GL4FrameBuffer& operator=(const GL4FrameBuffer& other) = delete;
		GL4FrameBuffer& operator=(GL4FrameBuffer&& other) noexcept;
		virtual ~GL4FrameBuffer();

		virtual void bind() override;
	};
}
