#pragma once

#include "Clove/Graphics/Core/GraphicsFactory.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace clv::gfx::d3d{
	class D3DGraphicsFactory : public GraphicsFactory{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dDeviceContext;

		//FUNCTIONS
	public:
		D3DGraphicsFactory() = delete;
		D3DGraphicsFactory(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dDeviceContext);

		D3DGraphicsFactory(const D3DGraphicsFactory& other) = delete;
		D3DGraphicsFactory(D3DGraphicsFactory&& other) = delete;

		D3DGraphicsFactory& operator=(const D3DGraphicsFactory& other) = delete;
		D3DGraphicsFactory& operator=(D3DGraphicsFactory&& other) = delete;

		virtual ~D3DGraphicsFactory();

		virtual std::shared_ptr<CommandBuffer> createCommandBuffer(const std::shared_ptr<RenderTarget>& renderTarget) override;
		virtual std::shared_ptr<CommandBuffer> createCommandBuffer(Surface& surface) override;

		virtual std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;

		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP) override;

		virtual std::shared_ptr<PipelineObject> createPipelineObject(const std::shared_ptr<Shader>& shader) override;

		virtual std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;

		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) override;

		virtual std::shared_ptr<Surface> createSurface(void* windowData) override;

	private:
		void onDeviceRemoved();
	};
}