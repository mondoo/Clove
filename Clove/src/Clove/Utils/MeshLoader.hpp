#pragma once

#include "Clove/Graphics/VertexLayout.hpp" //TODO: Remove? to get the input types

namespace clv{
	namespace loader{
		struct MeshInfo{
			std::vector<float> vertexData;
			std::vector<uint32> indices;

			std::vector<math::Vector3f> verticies;
			std::vector<math::Vector2f> texCoords;
			std::vector<math::Vector3f> normals;

		/*private:*/
			//this'll work if i could make the call a const value expression
			/*constexpr auto& get(gfx::VertexElementType layout){
				return getData<layout>();
			}*/
			///----

			//template<typename TupleType, int32 ...indices>
		/*private:*/
			template<gfx::VertexElementType layout>
			auto& getData(){
				if constexpr(layout == gfx::VertexElementType::position3D){
					return verticies;
				} else if constexpr(layout == gfx::VertexElementType::texture2D){
					return texCoords;
				} else if constexpr(layout == gfx::VertexElementType::normal){
					return normals;
				}
			}
		};

		class MeshLoader{
			//VARIABLES
		private:
			static MeshInfo emptyMeshInfo;
			static std::unordered_map<std::string, MeshInfo> loadedMeshes;

			//FUNCTIONS
		public:
			static const MeshInfo& loadOBJ(const std::string& filepath);

		};
	}
}