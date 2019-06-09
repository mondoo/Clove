//GL
#include "Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.hpp"

//DX
#include "Graphics/DirectX-11/Bindables/DX11ConstantBuffer.hpp"

namespace clv::gfx::BindableFactory{
	template<typename T>
	std::unique_ptr<ShaderBufferObject<T>> createShaderBufferObject(ShaderTypes shaderType, unsigned int bindingPoint){
		switch(Application::get().getRenderer().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4UniformBufferObject<T>>(bindingPoint);

			case API::DirectX11:
				switch(shaderType){
					case ShaderTypes::Vertex:
						return std::make_unique<DX11VertexConstantBuffer<T>>(bindingPoint);
						break;

					case ShaderTypes::Pixel:
						return std::make_unique<DX11PixelConstantBuffer<T>>(bindingPoint);
						break;

					default:
						CLV_ASSERT(false, "Unkown ShaderType in: " __FUNCTION__);
						return std::unique_ptr<ShaderBufferObject<T>>();
						break;
				}
				

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<ShaderBufferObject<T>>();
		}
	}
}