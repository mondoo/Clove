#pragma once

#include "Clove/Graphics/Bindables/FrameBuffer.hpp"

#include <wrl.h>

struct ID3D11RenderTargetView;

namespace clv::gfx{
	class DX11FrameBuffer : public FrameBuffer{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

		//FUNCTIONS
	public:
		DX11FrameBuffer();
		DX11FrameBuffer(const DX11FrameBuffer& other) = delete;
		DX11FrameBuffer(DX11FrameBuffer&& other) noexcept;
		DX11FrameBuffer& operator=(const DX11FrameBuffer& other) = delete;
		DX11FrameBuffer& operator=(DX11FrameBuffer&& other) noexcept;
		virtual ~DX11FrameBuffer();

		virtual void bind() override;

		virtual void attachTexture(Texture& texture) override;

		virtual bool isComplete() const override;
	};
}
