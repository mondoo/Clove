#pragma once

#include "Core/Graphics/RenderFactory.hpp"

#include <wrl.h>

struct ID3D11Device;

namespace clv::gfx::d3d::_11{
	class D3DRenderFactory : public RenderFactory{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;

		//FUNCTIONS
	public:
		D3DRenderFactory() = delete;
		D3DRenderFactory(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);
		D3DRenderFactory(const D3DRenderFactory& other) = delete;
		D3DRenderFactory(D3DRenderFactory&& other) = delete;
		D3DRenderFactory& operator=(const D3DRenderFactory& other) = delete;
		D3DRenderFactory& operator=(D3DRenderFactory&& other) = delete;
		~D3DRenderFactory();

		virtual std::shared_ptr<IndexBuffer> createIndexBuffer(const IndexBufferDescriptor& descriptor) override;
		virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const VertexBufferDescriptor& descriptor) override;

		virtual std::shared_ptr<ShaderResource> createShaderResource(const ShaderResourceDescriptor& descriptor) override;

		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor) override;

		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) override;

		virtual std::shared_ptr<Surface> createSurface() override;
	};
}