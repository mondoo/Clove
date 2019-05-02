#include "clvpch.hpp"
#include "Clove/Rendering/Renderer.hpp"

#include "Clove/Rendering/API/GLHelpers.hpp"
#include "Clove/Rendering/API/VertexArray.hpp"
#include "Clove/Rendering/API/IndexBuffer.hpp"
#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/Material.hpp"
#include "Clove/Scene/Scene.hpp"
#include "Clove/Scene/RenderableSceneNode.hpp"
#include "Clove/Scene/CameraSceneNode.hpp"
#include "Clove/Scene/Lights/PointLightSceneNode.hpp"
#include "Clove/Scene/Lights/DirectionalLightSceneNode.hpp"

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

	Renderer::~Renderer() = default;

	void Renderer::clear() const{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Renderer::submit(std::weak_ptr<scene::RenderableSceneNode> renderable){
		renderQueue.push(renderable);
	}

	void Renderer::drawQueue(std::shared_ptr<scene::Scene> currentScene){
		std::shared_ptr<scene::CameraSceneNode> camera = currentScene->getActiveCamera();

		if(!camera){
			CLV_ERROR("No active camera in current scene. Unable to draw current queue");
			while(!renderQueue.empty()){
				renderQueue.pop();
			}
			return;
		}

		currentShader->setUniform("viewPos", camera->getPosition());
		currentShader->setUniform("view", camera->getLookAt());
		currentShader->setUniform("projection", camera->getProjection());

		//NOTE: just having the default shader be effected by the lights
		std::shared_ptr<scene::DirectionalLightSceneNode> dirLight = currentScene->getDirectionalLights()[0]; //We only support one directional light
		defaultShader.setUniform("directionLight.direction", dirLight->getDirection());

		defaultShader.setUniform("directionLight.ambient", dirLight->getAmbientColour());
		defaultShader.setUniform("directionLight.diffuse", dirLight->getDiffuseColour());
		defaultShader.setUniform("directionLight.specular", dirLight->getSpecularColour());

		std::vector<std::shared_ptr<scene::PointLightSceneNode>> lights = currentScene->getPointLights();
		for(int i = 0; i < 4; ++i){ //We only support four lights currently
			defaultShader.setUniform("pointLights[" + std::to_string(i) + "].position", lights[i]->getPosition());

			defaultShader.setUniform("pointLights[" + std::to_string(i) + "].ambient", lights[i]->getAmbientColour());
			defaultShader.setUniform("pointLights[" + std::to_string(i) + "].diffuse", lights[i]->getDiffuseColour());
			defaultShader.setUniform("pointLights[" + std::to_string(i) + "].specular", lights[i]->getSpecularColour());

			defaultShader.setUniform("pointLights[" + std::to_string(i) + "].constant", lights[i]->getConstant());
			defaultShader.setUniform("pointLights[" + std::to_string(i) + "].linear", lights[i]->getLinear());
			defaultShader.setUniform("pointLights[" + std::to_string(i) + "].quadratic", lights[i]->getQuadratic());
		}

		while(!renderQueue.empty()){
			if(renderQueue.front().use_count() > 0){
				std::shared_ptr renderable = renderQueue.front().lock();

				prepareShader(renderable->getShaderType(), camera);

				currentShader->setUniform("model", renderable->getWorldTransform());

				renderable->bind(*currentShader);
				GLCall(glDrawElements(GL_TRIANGLES, renderable->getIndexBufferCount(), GL_UNSIGNED_INT, nullptr));
				renderable->unbind();

				renderQueue.pop();
			}
		}
	}

	void Renderer::prepareShader(ShaderType type, std::shared_ptr<scene::CameraSceneNode> camera){
		if(type != currentShaderType){
			switch(type){
				case ShaderType::standard:
					currentShader = &defaultShader;
					break;
				case ShaderType::light:
					currentShader = &lightShader;
					break;

				default:
					CLV_ERROR("Shader type {0} not supported! Using default shader", static_cast<int>(type));
					currentShader = &defaultShader;
					break;
			}

			currentShaderType = type;
			currentShader->bind();

			currentShader->setUniform("viewPos", camera->getPosition());
			currentShader->setUniform("view", camera->getLookAt());
			currentShader->setUniform("projection", camera->getProjection());
		}
	}
}