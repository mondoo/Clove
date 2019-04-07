#include "clvpch.hpp"
#include "Clove/MeshLoader.hpp"

#include <OBJ_Loader.h>

bool clv::loadOBJ(const std::string& filepath, MeshInfo& outVertexInfo){
	objl::Loader objLoader;
	if (objLoader.LoadFile(filepath)){
		outVertexInfo.verticies.clear();
		outVertexInfo.texCoords.clear();
		outVertexInfo.normals.clear();

		for (const auto& vertex : objLoader.LoadedVertices){
			outVertexInfo.verticies.push_back({ vertex.Position.X, vertex.Position.Y, vertex.Position.Z });
			outVertexInfo.texCoords.push_back({ vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y });
			outVertexInfo.normals.push_back({ vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z });
		}

		outVertexInfo.indices = objLoader.LoadedIndices;

		return true;
	} else{
		CLV_ERROR("Could not load file: {0}", filepath);
		return false;
	}
}
