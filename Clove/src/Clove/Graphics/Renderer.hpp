#pragma once

#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/RenderingTypes.hpp"

namespace clv{
	class Window;
	namespace scene{
		class Scene;
		class RenderableSceneNode;
		class CameraSceneNode;
	}

	namespace graphics{
		enum class API{
			OpenGL,
			DX11
		};

		class Renderer{
			//FUNCTIONS
		public:
			Renderer();
			Renderer(const Renderer& other) = delete;
			Renderer(Renderer&& other) noexcept = delete;

			virtual ~Renderer();

			virtual void clear() = 0;
			virtual void drawScene(std::shared_ptr<scene::Scene> scene) = 0;

			static Renderer* createOpenGLRenderer(const Window& window);
			static Renderer* createDirectX11Renderer(const Window& window);

			Renderer& operator=(const Renderer& other) = delete;
			Renderer& operator=(Renderer&& other) noexcept = delete;
		};
	}
}