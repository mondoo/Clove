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

		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, void* data) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, void* data, int32 BPP) override;
		virtual std::shared_ptr<PipelineObject> createPipelineObject(const std::shared_ptr<Shader>& shader) override;
		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;
		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) override;
		virtual std::shared_ptr<Surface> createSurface() override;

	private:
		void onDeviceRemoved();
	};
}