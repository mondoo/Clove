#include "clvpch.hpp"
#include "RendererContext.hpp"

#include "Clove/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"

#include "GraphicsAPI/GL/GLRenderer.hpp"
#include "GraphicsAPI/DX11/DX11Renderer.hpp"
#include "GraphicsAPI/GL/Bindables/GLVertexBuffer.hpp"
#include "GraphicsAPI/DX11/Bindables/DX11VertexBuffer.hpp"

namespace clv{
	namespace gfx{
		RendererContext::RendererContext() = default;

		RendererContext::~RendererContext() = default;

		void RendererContext::initialise(const Window& window, API api){
			switch(api){
				case API::OpenGL:
					CLV_TRACE("Creating OpenGL renderer");
					renderer = std::make_unique<GLRenderer>(window);
					break;

				case API::DirectX11:
					CLV_TRACE("Creating DirectX11 renderer");
					renderer = std::make_unique<DX11Renderer>(window);
					break;

				default:
					CLV_ERROR(__FUNCTION__ ": Default statement hit. No renderer initialised");
					break;
			}

			CLV_ASSERT(renderer != nullptr, "Renderer failed to initialise!");
			CLV_INFO("Renderer created succesfully");
		}

		void RendererContext::clear(){
			renderer->clear();
		}

		void RendererContext::drawScene(const std::shared_ptr<scene::Scene>& scene){
			//TODO: Do all the preperation in here then defer to the renderer
			//might not even need to take the scene (just the drawables)

			renderer->drawScene(scene);
		}

		std::unique_ptr<Bindable> RendererContext::createVertexBuffer(const std::vector<float>& vertices){
			std::unique_ptr<Bindable> vertexBuffer;

			switch(currentAPI){
				case API::OpenGL:
					vertexBuffer = std::make_unique<GLVertexBuffer>(vertices);
					break;

				case API::DirectX11:
					vertexBuffer = std::make_unique<DX11VertexBuffer>(vertices, renderer.get());
					break;

				default:
					CLV_ERROR(__FUNCTION__ ": Default statement hit. No vertex buffer initialised");
					break;
			}

			CLV_ASSERT(vertexBuffer != nullptr, "Failed to create a vertex buffer");
			CLV_TRACE("Vertex buffer created");

			return std::move(vertexBuffer);
		}
	}
}