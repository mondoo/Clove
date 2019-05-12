#pragma once

#include "Clove/Graphics/Renderer.hpp"

#include "GraphicsAPI/GL/OLD/Shader.hpp"

namespace clv{
	namespace gfx{
		class GLRenderer : public Renderer{
			//VARIABLES
		private:
		#if CLV_PLATFORM_WINDOWS
			HWND windowsHandle = nullptr;
			HDC windowsDeviceContext = nullptr;
			HGLRC windowsResourceContext = nullptr;
		#endif

			Shader* currentShader = nullptr;

			std::unique_ptr<Shader> defaultShader;
			std::unique_ptr<Shader> lightShader;

			ShaderType currentShaderType;

			size_t prevDirLightSize = 0;
			size_t prevPointLightSize = 0;

			//FUNCTIONS
		public:
			GLRenderer() = delete;
			GLRenderer(const GLRenderer& other) = delete;
			GLRenderer(GLRenderer&& other) noexcept = delete;

			GLRenderer(const Window& window);

			~GLRenderer();

			virtual void clear() override;
			virtual void drawScene(const std::shared_ptr<scene::Scene>& scene) override;

			GLRenderer& operator=(const GLRenderer& other) = delete;
			GLRenderer& operator=(GLRenderer&& other) noexcept = delete;

			//TODO: Move this back into the base renderer
		protected:
			void prepareShader(ShaderType type, std::shared_ptr<scene::CameraSceneNode> camera);
		};
	}
}

