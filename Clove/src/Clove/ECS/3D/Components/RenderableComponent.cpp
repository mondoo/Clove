#include "clvpch.hpp"
#include "RenderableComponent.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Context.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Utils/MeshLoader.hpp"

#include "Clove/Graphics/VertexLayout.hpp"

namespace clv::ecs::d3{
	RenderableComponent::RenderableComponent() = default;

	RenderableComponent::RenderableComponent(RenderableComponent&& other) noexcept = default;

	RenderableComponent& RenderableComponent::operator=(RenderableComponent&& other) noexcept = default;

	RenderableComponent::~RenderableComponent() = default;

	void RenderableComponent::setMesh(const std::string& filePath){
		loader::MeshInfo info = loader::MeshLoader::loadOBJ(filePath);

		gfx::VertexLayout layout;
		layout.add(gfx::VertexElementType::position3D).add(gfx::VertexElementType::texture2D).add(gfx::VertexElementType::normal);
		gfx::VertexBufferData vertexArray{ std::move(layout) };
		for(int i = 0; i < info.verticies.size(); ++i){
			vertexArray.emplaceBack(
				math::Vector3f{
					info.verticies[i].x,
					info.verticies[i].y,
					info.verticies[i].z
				},
				math::Vector2f{
					info.texCoords[i].x,
					info.texCoords[i].y
				},
				math::Vector3f{
					info.normals[i].x,
					info.normals[i].y,
					info.normals[i].z,
				}
			);
		}

		//Shader
		auto shader = gfx::BindableFactory::createShader(gfx::ShaderStyle::Lit);
		shader->bind();

		//VB
		std::shared_ptr<gfx::VertexBuffer> vertexBuffer = gfx::BindableFactory::createVertexBuffer(vertexArray, *shader);

		//IB
		submissionData.vertexBuffer = std::move(vertexBuffer);
		submissionData.indexBuffer = gfx::BindableFactory::createIndexBuffer(info.indices);
		submissionData.shader = std::move(shader);
	}

	void RenderableComponent::setAlbedoTexture(const std::string& path){
		submissionData.diffTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Albedo);
	}

	void RenderableComponent::setSpecularTexture(const std::string& path){
		submissionData.specTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Specular);
	}
}