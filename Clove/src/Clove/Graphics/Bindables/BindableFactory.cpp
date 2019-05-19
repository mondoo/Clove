#include "clvpch.hpp"
#include "BindableFactory.hpp"

#include "Clove/Graphics/Bindables/Bindable.hpp"
#include "Clove/Graphics/Renderer.hpp"

//GL
#include "Graphics/OpenGL/Bindables/GLVertexBuffer.hpp"
#include "Graphics/OpenGL/Bindables/GLIndexBuffer.hpp"
#include "Graphics/OpenGL/Bindables/GLShader.hpp"
#include "Graphics/OpenGL/Bindables/GLVertexBufferLayout.hpp"

//DX
#include "Graphics/DirectX-11/Bindables/DX11VertexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11IndexBuffer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Shader.hpp"
#include "Graphics/DirectX-11/Bindables/DX11VertexBufferLayout.hpp"

namespace clv::gfx{
	std::unique_ptr<Bindable> BindableFactory::createVertexBuffer(const std::vector<float>& vertexData, Renderer& renderer){
		switch(renderer.getAPI()){
			case API::OpenGL:
				return std::make_unique<GLVertexBuffer>(vertexData);

			case API::DirectX11:
				return std::make_unique<DX11VertexBuffer>(vertexData, renderer);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<Bindable>();
		}
	}

	std::unique_ptr<IndexBuffer> BindableFactory::createIndexBuffer(const std::vector<unsigned int>& indexData, Renderer& renderer){
		switch(renderer.getAPI()){
			case API::OpenGL:
				return std::make_unique<GLIndexBuffer>(indexData);

			case API::DirectX11:
				return std::make_unique<DX11IndexBuffer>(indexData, renderer);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<IndexBuffer>();
		}
	}

	std::unique_ptr<Shader> BindableFactory::createShader(Renderer& renderer){
		switch(renderer.getAPI()){
			case API::OpenGL:
				return std::make_unique<GLShader>();

			case API::DirectX11:
				return std::make_unique<DX11Shader>(renderer);

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<Shader>();
		}
	}

	std::unique_ptr<VertexBufferLayout> BindableFactory::createVertexBufferLayout(Renderer& renderer){
		switch(renderer.getAPI()){
			case API::OpenGL:
				return std::make_unique<GLVertexBufferLayout>();

			case API::DirectX11:
				return std::make_unique<DX11VertexBufferLayout>();

			default:
				CLV_ASSERT(false, "Unkown API in: " __FUNCTION__);
				return std::unique_ptr<VertexBufferLayout>();
		}
	}
}