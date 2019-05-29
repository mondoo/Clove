#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Graphics/OpenGL/GLRenderer.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#include "Clove/Graphics/Drawable.hpp"
#include "Clove/Scene/Camera.hpp"

namespace clv::gfx{
	Renderer::Renderer() = default;

	Renderer::~Renderer() = default;

	std::unique_ptr<Renderer> Renderer::createRenderer(const Window& window, API api){
		std::unique_ptr<Renderer> renderer;
		
		switch(api){
			case API::OpenGL:
				CLV_LOG_TRACE("Creating OpenGL renderer");
				renderer = std::make_unique<GLRenderer>(window);
				break;

			case API::DirectX11:
				CLV_LOG_TRACE("Creating DirectX11 renderer");
				renderer = std::make_unique<DX11Renderer>(window);
				break;

			default:
				CLV_LOG_ERROR(__FUNCTION__ ": Default statement hit. No renderer initialised");
				break;
		}

		if(renderer == nullptr){
			CLV_ASSERT(false, "Renderer failed to initialise!");
		} else{
			CLV_LOG_INFO("Renderer created succesfully");
		}

		return renderer;
	}

	void Renderer::submit(const std::shared_ptr<Drawable>& drawable){
		drawables.push(drawable);
	}

	void Renderer::draw(){
		while(drawables.size() > 0){
			if(std::shared_ptr<Drawable> drawable = drawables.front().lock()){
				drawable->draw(*this);
				drawables.pop();
			}
		}

		swapBuffers();
	}

	void Renderer::setActiveCamera(scene::Camera* cam){
		CLV_ASSERT(activeCamera == nullptr, "Active camera already set!");
		activeCamera = cam;
	}
}