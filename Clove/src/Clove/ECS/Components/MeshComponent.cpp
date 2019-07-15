#include "clvpch.hpp"
#include "MeshComponent.hpp"

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

namespace clv::ecs{
	MeshComponent::MeshComponent() = default;

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept = default;

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept = default;

	MeshComponent::~MeshComponent() = default;

	void MeshComponent::setMesh(const std::string& filePath){
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

		for(const auto i : info.indices){
			indices.push_back(i);
		}

		//Shader
		std::shared_ptr<gfx::Shader> shader = gfx::BindableFactory::createShader();
		shader->attachShader(gfx::ShaderType::Vertex);
		shader->attachShader(gfx::ShaderType::Pixel);
		shader->bind();

		//VB
		std::shared_ptr<gfx::VertexBuffer> vertexBuffer = gfx::BindableFactory::createVertexBuffer(vertexArray, *shader);

		//IB
		addIndexBuffer(gfx::BindableFactory::createIndexBuffer(indices));

		addBindable(std::move(vertexBuffer));
		addBindable(std::move(shader));
	}

	void MeshComponent::setDiffuseTexture(const std::string& path){
		addBindable(gfx::BindableFactory::createTexture(path, gfx::TBP_Diffuse));
	}

	void MeshComponent::setSpecularTexture(const std::string& path){
		addBindable(gfx::BindableFactory::createTexture(path, gfx::TBP_Specular));
	}
}