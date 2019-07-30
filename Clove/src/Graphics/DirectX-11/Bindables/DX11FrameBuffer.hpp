#pragma once

#include "Clove/Graphics/Bindables/FrameBuffer.hpp"

namespace clv::gfx{
	class DX11FrameBuffer : public FrameBuffer{
		//VARIABLES
	private:

		//TODO

		//FUNCTIONS
	public:
		DX11FrameBuffer();
		DX11FrameBuffer(const DX11FrameBuffer& other) = delete;
		DX11FrameBuffer(DX11FrameBuffer&& other) noexcept;
		DX11FrameBuffer& operator=(const DX11FrameBuffer& other) = delete;
		DX11FrameBuffer& operator=(DX11FrameBuffer&& other) noexcept;
		virtual ~DX11FrameBuffer();

		virtual void bind() override;
	};
}
