#pragma once

#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/Renderable.hpp"

#include <queue>

namespace clv{
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

		//TODO: this works for now with one shader but will need to be revisited when others are created
		template<typename T>
		void setGlobalShaderUniform(const std::string& name, const T& value);
		// Ideally, things like lighting will be handled in a 'light' object then that data can be sent to the shader

	private:
		void prepareShader(ShaderType type);
	};
}

#include "Renderer.inl"