#include "clvpch.hpp"
#include "Render3DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
//#include "Clove/Graphics/Bindable.hpp"
//#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
//#include "Clove/Graphics/Bindables/Shader.hpp"
//#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::ecs{
	Render3DSystem::Render3DSystem() = default;

	Render3DSystem::Render3DSystem(Render3DSystem&& other) noexcept = default;

	Render3DSystem& Render3DSystem::operator=(Render3DSystem&& other) noexcept = default;

	Render3DSystem::~Render3DSystem() = default;

	void Render3DSystem::update(float deltaTime){
		//Note: How do I want the below to look?
		//I like the idea of the system just parsing the data then giving it to the renderer
		//	-but what will the renderer need?

		//renderer->clear(); //NOTE: putting it here will clear the imgui shiz

		for(auto& componentTuple : components){
			Transform3DComponent* transform = std::get<Transform3DComponent*>(componentTuple);
			Renderable3DComponent* renderable = std::get<Renderable3DComponent*>(componentTuple);

			const math::Matrix4f modelMat = getTransformWorldMatrix(transform);

			gfx::Renderer::submitMesh(modelMat, renderable->indexBuffer, renderable->bindables);
			
			/*for(const auto& bindable : renderable->bindables){
				bindable->bind(*renderer);
			}*/

			//renderer->drawIndexed(renderable->indexBuffer->getIndexCount());
		}
	}

	//void Render3DSystem::initialiseRenderer(const gfx::Context& context){
	//	renderer = gfx::Renderer::createRenderer(context);

	//	//TODO: Temp?
	//	vertCB = gfx::BindableFactory::createShaderBufferObject<VertexData>(gfx::ShaderTypes::Vertex, gfx::BBP_ModelData);
	//	materialCB = gfx::BindableFactory::createShaderBufferObject<MaterialData>(gfx::ShaderTypes::Pixel, gfx::BBP_MaterialData);
	//
	//	mData.sininess = 32.0f;
	//	materialCB->update(mData, *renderer);
	//	//
	//}

	/*gfx::Renderer& Render3DSystem::getRenderer(){
		return *renderer;
	}*/

	math::Matrix4f Render3DSystem::getTransformWorldMatrix(Transform3DComponent* component){
		const auto& [rot, angle] = component->getLocalRotation();

		math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), component->getLocalPosition());
		math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), angle, rot);
		math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), component->getLocalScale());

		math::Matrix4f transform = translation * rotation * scale;

		if(Transform3DComponent* parent = component->getParent()){
			return getTransformWorldMatrix(parent) * transform;
		} else{
			return transform;
		}
	}
}