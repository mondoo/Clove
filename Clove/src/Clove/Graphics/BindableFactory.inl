#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"

//GL
#include "Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.hpp"

#if CLV_PLATFORM_WINDOWS
//DX
#include "Graphics/DirectX-11/Bindables/DX11ConstantBuffer.hpp"
#endif

namespace clv::gfx::BindableFactory{
	template<typename T>
	std::unique_ptr<ShaderBufferObject<T>> createShaderBufferObject(ShaderTypes shaderType, unsigned int bindingPoint){
		switch(Application::get().getRenderer().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4UniformBufferObject<T>>(bindingPoint);

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				switch(shaderType){
					case ShaderTypes::Vertex:
						return std::make_unique<DX11VertexConstantBuffer<T>>(bindingPoint);
						break;

					case ShaderTypes::Pixel:
						return std::make_unique<DX11PixelConstantBuffer<T>>(bindingPoint);
						break;

					default:
						CLV_ASSERT(false, "Unkown ShaderType in: {0}", __func__);
						return std::unique_ptr<ShaderBufferObject<T>>();
						break;
				}
			#endif	

			default:
				CLV_ASSERT(false, "Unkown API in: {0}", __func__);
				return std::unique_ptr<ShaderBufferObject<T>>();
		}
	}
}