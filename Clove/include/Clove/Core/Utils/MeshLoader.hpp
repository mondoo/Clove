#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::loader{
	struct MeshInfo{
		std::vector<float> vertexData;
		std::vector<uint32_t> indices;

		std::vector<mth::vec3f> verticies;
		std::vector<mth::vec2f> texCoords;
		std::vector<mth::vec3f> normals;

		template<gfx::VertexElementType layout>
		auto& getData(){
			if constexpr (layout == gfx::VertexElementType::position3D){
				return verticies;
			} else if constexpr (layout == gfx::VertexElementType::texture2D){
				return texCoords;
			} else if constexpr (layout == gfx::VertexElementType::normal){
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