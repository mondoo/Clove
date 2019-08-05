#include "clvpch.hpp"
#include "Mesh.hpp"

#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Utils/MeshLoader.hpp"

namespace clv::gfx{
	Mesh::Mesh() = default;

	Mesh::Mesh(const Mesh& other) = default;

	Mesh& Mesh::operator=(const Mesh& other) = default;

	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

	Mesh::Mesh(std::string filePath){
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
		shader = gfx::BindableFactory::createShader(gfx::ShaderStyle::Lit);
		shader->bind();

		//VB
		vertexBuffer = gfx::BindableFactory::createVertexBuffer(vertexArray, *shader);

		//IB
		indexBuffer = gfx::BindableFactory::createIndexBuffer(info.indices);
	}

	void Mesh::setAlbedoTexture(const std::string& path){
		albedoTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Albedo);
	}

	void Mesh::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		albedoTexture = texture;
	}

	void Mesh::setSpecularTexture(const std::string& path){
		specTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Specular);
	}

	void Mesh::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		specTexture = texture;
	}

	void Mesh::bind(){
		vertexBuffer->bind();
		indexBuffer->bind();
		shader->bind();
		albedoTexture->bind();
		if(specTexture){
			specTexture->bind();
		}
	}
}