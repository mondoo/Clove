#pragma once

#include "Clove/Graphics/Renderer.hpp"

namespace clv{
	namespace graphics{
		class OGLRenderer : public Renderer{
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
			OGLRenderer() = delete;
			OGLRenderer(const OGLRenderer& other) = delete;
			OGLRenderer(OGLRenderer&& other) noexcept = delete;

			OGLRenderer(const Window& window);

			~OGLRenderer();

			virtual void clear() override;
			virtual void drawScene(std::shared_ptr<scene::Scene> scene) override;

			OGLRenderer& operator=(const OGLRenderer& other) = delete;
			OGLRenderer& operator=(OGLRenderer&& other) noexcept = delete;

			//TODO: Move this back into the base renderer
		protected:
			void prepareShader(ShaderType type, std::shared_ptr<scene::CameraSceneNode> camera);
		};
	}
}

