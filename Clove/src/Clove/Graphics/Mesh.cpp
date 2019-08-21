#include "clvpch.hpp"
#include "Mesh.hpp"

#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
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

		//Shader
		shader = gfx::BindableFactory::createShader(gfx::ShaderStyle::Lit);
		shader->bind();

		const int32 vertexCount = info.verticies.size();

		gfx::VertexLayout layout = shader->getReflectionData().vertexBufferLayout;
		gfx::VertexBufferData vertexArray{ layout };
		vertexArray.resize(vertexCount);
		
		for(int32 i = 0; i < vertexCount; ++i){
			for(int32 j = 0; j < layout.count(); ++j){
				switch(layout.resolve(j).getType()){
					case VertexElementType::position3D:
						vertexArray[i].getAttribute<VertexElementType::position3D>() = math::Vector3f{ info.getData<VertexElementType::position3D>()[i] };
						break;

					case VertexElementType::texture2D:
						vertexArray[i].getAttribute<VertexElementType::texture2D>() = math::Vector2f{ info.getData<VertexElementType::texture2D>()[i] };
						break;

					case VertexElementType::normal:
						vertexArray[i].getAttribute<VertexElementType::normal>() = math::Vector3f{ info.getData<VertexElementType::normal>()[i] };
						break;
					default:
						break;
				}
			}
		}

		//VB
		vertexBuffer = gfx::BindableFactory::createVertexBuffer(vertexArray, *shader);

		//IB
		indexBuffer = gfx::BindableFactory::createIndexBuffer(info.indices);
	}

	Mesh::Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, ShaderStyle shaderStyle){
		//Shader
		shader = gfx::BindableFactory::createShader(shaderStyle);
		shader->bind();

		//VB
		vertexBuffer = gfx::BindableFactory::createVertexBuffer(vbData, *shader);

		//IB
		indexBuffer = gfx::BindableFactory::createIndexBuffer(indices);
	}

	void Mesh::setMaterial(const std::shared_ptr<Material>& material){
		this->material = material;
		/*
		TODO: Link shader to material here
		*/
		//This is just a temp test
		material->linkShader(shader);

		/*
		This will take a material instance, so the shader should probably belong in the material
		 */
	}

	const std::shared_ptr<Material>& Mesh::getMaterial() const{
		return material;
	}

	uint32 Mesh::getIndexCount(){
		return indexBuffer->getIndexCount();
	}

	void Mesh::bind(){
		vertexBuffer->bind();
		indexBuffer->bind();
		if(material){ //Can be null - should we use a different mesh type?
			material->bind();
		}
		shader->bind();
	}
}