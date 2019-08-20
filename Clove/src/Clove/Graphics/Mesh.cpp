#include "clvpch.hpp"
#include "Mesh.hpp"

#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
//#include "Clove/Graphics/VertexLayout.hpp"
//#include "Clove/Utils/MeshLoader.hpp"

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

		gfx::VertexLayout layout = shader->getReflectionData().vertexBufferLayout;
		gfx::VertexBufferData vertexArray{ layout };
		
		//constexpr size_t layoutNum = layout.count();
		for(int i = 0; i < info.verticies.size(); ++i){
			//TODO: We'd either need a system to retrieve only the data that the shader can take or choose a shader depending on what the mesh has
			auto vertices	= math::Vector3f{ info.getData<VertexElementType::position3D>()[i] };
			auto texCoords	= math::Vector2f{ info.getData<VertexElementType::texture2D>()[i] };
			auto normals	= math::Vector3f{ info.getData<VertexElementType::normal>()[i] };

			/*
			I can resolve an element at compile time VertexLayout::resolve
			so I could a resize the buffer by 1, get the back vertex, loop through the layout elements, do a switch statement on the type, then set the vertex element to that?
			*/

			vertexArray.addOne();
			for(int32 j = 0; j < layout.count(); ++j){
				switch(layout.resolve(j).getType()){
					case VertexElementType::position3D:
						vertexArray.back().setAttributeByIndex(j, math::Vector3f{ info.getData<VertexElementType::position3D>()[i] });
						break;

					case VertexElementType::texture2D:
						vertexArray.back().setAttributeByIndex(j, math::Vector2f{ info.getData<VertexElementType::texture2D>()[i] });
						break;

					case VertexElementType::normal:
						vertexArray.back().setAttributeByIndex(j, math::Vector3f{ info.getData<VertexElementType::normal>()[i] });
						break;
					default:
						break;
				}
			}

			/****
			anyway we can use a tuple for this?
			****/

			/*auto func = [vertices, texCoords, normals](VertexElementType elemType) -> auto{
				if (elemType == VertexElementType::position3D){
					return vertices;
				} else if(elemType == VertexElementType::texture2D){
					return texCoords;
				}
			}*/

			//In DX these are a different order?
			//How do I change the order in which these are passed depending on the order that the layout is built
			//vertexArray.emplaceBack(info.getData<layout.resolve(0).getType()>(), vertices, texCoords);

			//setVertex(vertexArray, info, layout, i, std::make_integer_sequence<int32, 3>());


			/*
			something is going to have to change if I want this.
			The main problem that I've found is that there is no way to dynamically get the type of element at compile time

			*/
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