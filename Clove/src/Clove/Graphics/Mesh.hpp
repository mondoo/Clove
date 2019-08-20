#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/VertexLayout.hpp" //needed for inl function
#include "Clove/Utils/MeshLoader.hpp" //needed for inl function

namespace clv::gfx{
	class VertexBuffer;
	class IndexBuffer;
	class Material;
	class Shader;
	class VertexBufferData;
	
	class Mesh{
		//VARIABLES
	private:
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<Material> material;
		std::shared_ptr<Shader> shader;//Temp????

		//FUNCTIONS
	public:
		Mesh();
		Mesh(const Mesh& other);
		Mesh& operator=(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		//TODO: I think the constructor should take the material instance. Then we can retrieve the shader from the material
		Mesh(std::string filePath);
		Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, ShaderStyle shaderStyle);

		void setMaterial(const std::shared_ptr<Material>& material);
		const std::shared_ptr<Material>& getMaterial() const;

		uint32 getIndexCount();

		void bind();

	private:
		//template<int32 ...layoutIndices>
		//void setVertex(gfx::VertexBufferData& buffer, const loader::MeshInfo& info, const gfx::VertexLayout& layout, int32 vertexIndex, std::integer_sequence<int32, layoutIndices...>){
		//	//This wouldn't work because we'd need to convert it to the right type
		//	//Unlesss...
		//	//buffer.emplaceBack(VertexElementData<layout.resolve(layoutIndices + 1).getType()>::DataType{ info.getData<layout.resolve(layoutIndices + 1).getType()>()[vertexIndex] }...);
		//}

		//template<typename ...Args>
		//auto makeTuple(Args&& ...args){

		//}

		//template<int32 ...layoutIndices>
		//void setVertex(gfx::VertexBufferData& buffer, const loader::MeshInfo& info, const gfx::VertexLayout& layout, int32 vertexIndex, std::integer_sequence<int32, layoutIndices...>){
		//	//If i make a tuple then I could do something similar to above

		//	//tuple size is a constexpr
		//}

		//make a single tuple for each elem

		/****
		Is there a way I can resize the whole buffer and then set each vertex attrib?
		*****/
	};
}
