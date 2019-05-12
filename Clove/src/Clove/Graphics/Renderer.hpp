#pragma once

#include "Clove/Graphics/RenderingTypes.hpp"

namespace clv{
	class Window;
	namespace scene{
		class Scene;
		class RenderableSceneNode;
		class CameraSceneNode;
	}

	namespace gfx{
		class Renderer{
			//FUNCTIONS
		public:
			Renderer();
			Renderer(const Renderer& other) = delete;
			Renderer(Renderer&& other) noexcept = delete;
			Renderer& operator=(const Renderer& other) = delete;
			Renderer& operator=(Renderer&& other) noexcept = delete;
			virtual ~Renderer();

			virtual void clear() = 0;
			virtual void drawScene(const std::shared_ptr<scene::Scene>& scene) = 0;
		};
	}
}