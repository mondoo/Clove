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

		//The shader reflection system can get this information.
		//I think the flow should then be the VBD gets build from the shader reflection
		//then passed onto the vertex buffer
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

		//TODO: Need shader from material
		//Or do i give the shader to the material?
		//..
		/*
		Is there anyway I can seperate out the inputlayout from the vertex buffer? (and subsiquently the vertex arrays)
		-The shader below is just for the input layout for the vertex buffer but I think this could be generated seperately
		--This means the material could just bind it and we don't have to worry about giving it to the vertex buffer
		-Might be difficult because they both need the same vertex buffer data (vb needs the actual data and the layout needs the layout info)
		
		*/

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