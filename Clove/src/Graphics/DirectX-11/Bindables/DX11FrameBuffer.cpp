#include "clvpch.hpp"
#include "DX11FrameBuffer.hpp"

namespace clv::gfx{
	DX11FrameBuffer::DX11FrameBuffer() = default;

	DX11FrameBuffer::DX11FrameBuffer(DX11FrameBuffer&& other) noexcept = default;

	DX11FrameBuffer& DX11FrameBuffer::operator=(DX11FrameBuffer&& other) noexcept = default;

	DX11FrameBuffer::~DX11FrameBuffer() = default;

	void DX11FrameBuffer::bind(){
		//TODO
	}
}