#pragma once

#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/Renderable.hpp"

#include <queue>

namespace clv{
	class VertexArray;
	class IndexBuffer;
	class Material;

	class Renderer{
		//VARIABLES
	private:
		Shader* currentShader = nullptr;
		Shader defaultShader;

		ShaderType currentShaderType;

		std::queue<std::weak_ptr<Renderable>> renderQueue;

		//FUNCTIONS
	public:
		Renderer();

		void clear() const;

		void submit(std::weak_ptr<Renderable> renderable);
		void drawQueue();

	private:
		void prepareShader(ShaderType type);
	};
}