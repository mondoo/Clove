#include "clvpch.hpp"
#include "BindableFactory.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Bindables/Bindable.hpp"
#include "Clove/Graphics/Renderer.hpp"

//GL
#include "Graphics/OpenGL/Bindables/GLVertexBuffer.hpp"
#include "Graphics/OpenGL/Bindables/GLIndexBuffer.hpp"
#include "Graphics/OpenGL/Bindables/GLShader.hpp"
#include "Graphics/OpenGL/Bindables/GLVertexBufferLayout.hpp"
#include "Graphics/OpenGL/Bindables/GLTexture.hpp"

//DX
#include "Graphics/DirectX-11/Bindables/DX11VertexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11IndexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Shader.hpp"
#include "Graphics/DirectX-11/Bindables/DX11VertexBufferLayout.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Texture.hpp"

namespace clv::gfx::BindableFactory{
	std::unique_ptr<Bindable> createVertexBuffer(const std::vector<float>& vertexData){
		switch(Application::get().getWindow().getRenderer().getAPI()){
			case API::OpenGL:
				return std::make_unique<GLVertexBuffer>(vertexData);

			case API::DirectX11:
				return std::make_unique<DX11VertexBuffer>(vertexData);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<Bindable>();
		}
	}

	std::unique_ptr<IndexBuffer> createIndexBuffer(const std::vector<unsigned int>& indexData){
		switch(Application::get().getWindow().getRenderer().getAPI()){
			case API::OpenGL:
				return std::make_unique<GLIndexBuffer>(indexData);

			case API::DirectX11:
				return std::make_unique<DX11IndexBuffer>(indexData);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<IndexBuffer>();
		}
	}

	std::unique_ptr<Shader> createShader(){
		switch(Application::get().getWindow().getRenderer().getAPI()){
			case API::OpenGL:
				return std::make_unique<GLShader>();

			case API::DirectX11:
				return std::make_unique<DX11Shader>();

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<Shader>();
		}
	}

	std::unique_ptr<VertexBufferLayout> createVertexBufferLayout(){
		switch(Application::get().getWindow().getRenderer().getAPI()){
			case API::OpenGL:
				return std::make_unique<GLVertexBufferLayout>();

			case API::DirectX11:
				return std::make_unique<DX11VertexBufferLayout>();

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<VertexBufferLayout>();
		}
	}

	std::unique_ptr<Texture> createTexture(const std::string& filePath){
		switch(Application::get().getWindow().getRenderer().getAPI()){
			case API::OpenGL:
				return std::make_unique<GLTexture>(filePath);

			case API::DirectX11:
				return std::make_unique<DX11Texture>(filePath);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<Texture>();
		}
	}
}