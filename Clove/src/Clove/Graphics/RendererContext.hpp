#pragma once

#include "Clove/Graphics/RenderingTypes.hpp"

namespace clv{
	class Window;

	namespace scene{
		class Scene;
	}

	namespace gfx{
		class Renderer;
		class Bindable;

		class RendererContext{
			//VARIABLES
		private:
			API currentAPI;

			std::unique_ptr<Renderer> renderer;

			//FUNCTIONS
		public:
			RendererContext();
			RendererContext(const RendererContext& other) = delete;
			RendererContext(RendererContext&& other) noexcept = delete;
			RendererContext& operator=(const RendererContext& other) = delete;
			RendererContext& operator=(RendererContext&& other) noexcept = delete;
			~RendererContext();

			void initialise(const Window& window, API api);

			void clear();
			void drawScene(const std::shared_ptr<scene::Scene>& scene);

			//TODO: Is there a way to achieve this without calling off to the context?
			//Maybe have some free functions somewhere?
			std::unique_ptr<Bindable> createVertexBuffer(const std::vector<float>& vertices);
		};
	}
}