#pragma once

#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/RenderingTypes.hpp"

#include <queue>

namespace clv{
	namespace scene{
		class Scene;
		class RenderableSceneNode;
		class CameraSceneNode;
	}

	class Renderer{
		//VARIABLES
	private:
		Shader* currentShader = nullptr;

		Shader defaultShader;
		Shader lightShader;

		ShaderType currentShaderType;

		std::queue<std::weak_ptr<scene::RenderableSceneNode>> renderQueue;

		size_t prevDirLightSize = 0;
		size_t prevPointLightSize = 0;

		//FUNCTIONS
	public:
		Renderer();

		~Renderer();

		void clear() const;
		void submit(std::weak_ptr<scene::RenderableSceneNode> renderable);
		void drawQueue(std::shared_ptr<scene::Scene> currentScene);

	private:
		void prepareShader(ShaderType type, std::shared_ptr<scene::CameraSceneNode> camera);
	};
}

#include "Renderer.inl"