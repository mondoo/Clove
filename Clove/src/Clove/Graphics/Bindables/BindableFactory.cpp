#include "clvpch.hpp"
#include "BindableFactory.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"

//GL
#include "Graphics/OpenGL-4/Bindables/GL4VertexBuffer.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4IndexBuffer.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4Shader.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4VertexBufferLayout.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4Texture.hpp"

//DX
#include "Graphics/DirectX-11/Bindables/DX11VertexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11IndexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Shader.hpp"
#include "Graphics/DirectX-11/Bindables/DX11VertexBufferLayout.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Texture.hpp"

namespace clv::gfx::BindableFactory{
	std::unique_ptr<VertexBuffer> createVertexBuffer(const std::vector<Vertex>& vertexData){
		switch(Application::get().getRenderer().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4VertexBuffer>(vertexData);

			case API::DirectX11:
				return std::make_unique<DX11VertexBuffer>(vertexData);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<VertexBuffer>();
		}
	}

	std::unique_ptr<IndexBuffer> createIndexBuffer(const std::vector<unsigned int>& indexData){
		switch(Application::get().getRenderer().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4IndexBuffer>(indexData);

			case API::DirectX11:
				return std::make_unique<DX11IndexBuffer>(indexData);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<IndexBuffer>();
		}
	}

	std::unique_ptr<Shader> createShader(){
		switch(Application::get().getRenderer().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4Shader>();

			case API::DirectX11:
				return std::make_unique<DX11Shader>();

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<Shader>();
		}
	}

	std::unique_ptr<VertexBufferLayout> createVertexBufferLayout(){
		switch(Application::get().getRenderer().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GLVertexBufferLayout>();

			case API::DirectX11:
				return std::make_unique<DX11VertexBufferLayout>();

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<VertexBufferLayout>();
		}
	}

	std::unique_ptr<Texture> createTexture(const std::string& filePath, unsigned int bindingPoint){
		switch(Application::get().getRenderer().getAPI()){
			case API::OpenGL4:
				return std::make_unique<GL4Texture>(filePath, bindingPoint);

			case API::DirectX11:
				return std::make_unique<DX11Texture>(filePath, bindingPoint);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<Texture>();
		}
	}
}