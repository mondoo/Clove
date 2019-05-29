#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include <queue>

namespace clv{
	class Window;
}

namespace clv::scene{
	class Camera;
}

namespace clv::gfx{
	class Drawable;

	class Renderer{
		friend class Drawable;

		//VARIABLES
	private:
		std::queue<std::weak_ptr<Drawable>> drawables;

		scene::Camera* activeCamera = nullptr;

		//FUNCTIONS
	public:
		Renderer();
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) noexcept = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) noexcept = delete;
		virtual ~Renderer();

		static std::unique_ptr<Renderer> createRenderer(const Window& window, API api);

		void submit(const std::shared_ptr<Drawable>& drawable);

		virtual void clear() = 0;
		void draw();

		inline virtual API getAPI() const = 0;

		void setActiveCamera(scene::Camera* cam);
		inline const scene::Camera& getActiveCamera() const; //TODO: the renderer was supposed to give the camera to the drawables, is this better?

	protected:
		virtual void drawIndexed(const unsigned int count) = 0;

		virtual void swapBuffers() = 0;
	};
}

#include "Renderer.inl"