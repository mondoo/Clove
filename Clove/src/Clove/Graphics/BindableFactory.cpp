#include "clvpch.hpp"
#include "BindableFactory.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Context.hpp"

//GL
#include "Graphics/OpenGL-4/Bindables/GL4VertexBuffer.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4IndexBuffer.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4Shader.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4VertexBufferLayout.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4Texture.hpp"

//DX
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/Bindables/DX11VertexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11IndexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Shader.hpp"
#include "Graphics/DirectX-11/Bindables/DX11VertexBufferLayout.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Texture.hpp"
#endif

namespace clv::gfx::BindableFactory{
	//std::unique_ptr<VertexBuffer> createVertexBuffer(const std::vector<Vertex>& vertexData){
	//	switch(Application::get().getWindow().getContext().getAPI()){
	//		case API::OpenGL4:
	//			return std::make_unique<GL4VertexBuffer>(vertexData);

	//		#if CLV_PLATFORM_WINDOWS
	//		case API::DirectX11:
	//			return std::make_unique<DX11VertexBuffer>(vertexData);
	//		#endif

	//		default:
	//			CLV_ASSERT(false, "Unkown API in: {0}", __func__);
	//			return std::unique_ptr<VertexBuffer>();
	//	}
	//}

	std::unique_ptr<IndexBuffer> createIndexBuffer(const std::vector<unsigned int>& indexData){
		switch(Application::get().getWindow().getContext().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4IndexBuffer>(indexData);

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				return std::make_unique<DX11IndexBuffer>(indexData);
			#endif

			default:
				CLV_ASSERT(false, "Unkown API in: {0}", __func__);
				return std::unique_ptr<IndexBuffer>();
		}
	}

	std::unique_ptr<Shader> createShader(){
		switch(Application::get().getWindow().getContext().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4Shader>();

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				return std::make_unique<DX11Shader>();
			#endif

			default:
				CLV_ASSERT(false, "Unkown API in: {0}", __func__);
				return std::unique_ptr<Shader>();
		}
	}

	//std::unique_ptr<VertexLayout> createVertexBufferLayout(){
	//	switch(Application::get().getWindow().getContext().getAPI()){
	//		case API::OpenGL4:
	//			return std::make_unique<GLVertexBufferLayout>();

	//		#if CLV_PLATFORM_WINDOWS
	//		case API::DirectX11:
	//			return std::make_unique<DX11VertexBufferLayout>();
	//		#endif

	//		default:
	//			CLV_ASSERT(false, "Unkown API in: {0}", __func__);
	//			return std::unique_ptr<VertexLayout>();
	//	}
	//}

	std::unique_ptr<Texture> createTexture(const std::string& filePath, unsigned int bindingPoint){
		switch(Application::get().getWindow().getContext().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4Texture>(filePath, bindingPoint);

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				return std::make_unique<DX11Texture>(filePath, bindingPoint);
			#endif

			default:
				CLV_ASSERT(false, "Unkown API in: {0}", __func__);
				return std::unique_ptr<Texture>();
		}
	}
}