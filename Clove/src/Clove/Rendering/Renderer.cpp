#include "clvpch.hpp"
#include "Clove/Rendering/Renderer.hpp"

#include "Clove/Rendering/API/GLHelpers.hpp"

#include "Clove/Rendering/API/VertexArray.hpp"
#include "Clove/Rendering/API/IndexBuffer.hpp"
#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/Material.hpp"

#include <glad/glad.h>

namespace clv{
	Renderer::Renderer(){
		defaultShader.attachShader(ShaderTypes::Vertex, "../Clove/res/Shaders/VertexShader.glsl");
		defaultShader.attachShader(ShaderTypes::Fragment, "../Clove/res/Shaders/FragmentShader.glsl");

		lightShader.attachShader(ShaderTypes::Vertex, "../Clove/res/Shaders/VertexShader.glsl");
		lightShader.attachShader(ShaderTypes::Fragment, "../Clove/res/Shaders/LightFragShader.glsl");

		currentShaderType = ShaderType::standard;

		currentShader = &defaultShader;
		currentShader->bind();
	}

	void Renderer::clear() const{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Renderer::submit(std::weak_ptr<Renderable> renderable){
		renderQueue.push(renderable);
	}

	void Renderer::drawQueue(){
		while(!renderQueue.empty()){
			if(!renderQueue.front().expired()){
				std::shared_ptr renderable = renderQueue.front().lock();

				prepareShader(renderable->getShaderType());
				
				renderable->bind(*currentShader);
				GLCall(glDrawElements(GL_TRIANGLES, renderable->getIndexBufferCount(), GL_UNSIGNED_INT, nullptr));
				renderable->unbind();

				renderQueue.pop();
			}
		}
	}

	void Renderer::prepareShader(ShaderType type){
		if(type != currentShaderType){
			switch(type){
			case ShaderType::standard:
				currentShader = &defaultShader;
				break;
			case ShaderType::light:
				currentShader = &lightShader;
				break;

			default:
				CLV_ASSERT(false, "Shader type not supported!");
				break;
			}

			currentShaderType = type;
			currentShader->bind();
		}
	}
}