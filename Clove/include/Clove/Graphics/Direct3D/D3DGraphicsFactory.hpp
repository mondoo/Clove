#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace clv::gfx::d3d {
	class D3DGraphicsFactory : public GraphicsFactory {
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

		~D3DGraphicsFactory();

		std::shared_ptr<CommandBuffer> createCommandBuffer() override;

		std::shared_ptr<Buffer> createBuffer(const BufferDescriptor& descriptor, const void* data) override;

		std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture) override;
		std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP) override;

		std::shared_ptr<PipelineObject> createPipelineObject() override;

		std::shared_ptr<RenderTarget> createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture) override;

		std::shared_ptr<Shader> createShader(ShaderDescriptor descriptor, std::string_view pathToShader) override;
		std::shared_ptr<Shader> createShader(ShaderDescriptor descriptor, const char* bytes, const std::size_t size) override;

		std::shared_ptr<Surface> createSurface(void* windowData) override;

	#if CLV_IMGUI_COMPATIBLE
		const Microsoft::WRL::ComPtr<ID3D11Device>& getDevice() const;
		const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& getContext() const;
	#endif

	private:
		void onDeviceRemoved();
	};
}