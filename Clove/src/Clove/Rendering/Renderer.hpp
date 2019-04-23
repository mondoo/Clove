#pragma once

#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Scene/RenderableSceneNode.hpp"

#include <queue>

namespace clv{
	namespace scene{ 
		class Scene; 
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

		//FUNCTIONS
	public:
		Renderer();

		void clear() const;
		void submit(std::weak_ptr<scene::RenderableSceneNode> renderable);
		void drawQueue(std::shared_ptr<scene::Scene> currentScene);

	private:
		void prepareShader(ShaderType type, std::shared_ptr<scene::CameraSceneNode> camera);
	};
}

#include "Renderer.inl"