#include "MeshLoader.hpp"

#include <OBJ_Loader.h>

namespace clv{
	namespace loader{
		MeshInfo MeshLoader::emptyMeshInfo = MeshInfo();
		std::unordered_map<std::string, MeshInfo> MeshLoader::loadedMeshes = std::unordered_map<std::string, MeshInfo>();

		const MeshInfo& MeshLoader::loadOBJ(const std::string& filepath){
			auto mapIt = loadedMeshes.find(filepath);
			if(mapIt != loadedMeshes.end()){
				CLV_LOG_TRACE("Reusing model data for: {0}", filepath);

				return mapIt->second;
			} else{
				CLV_LOG_WARN("Creating a new model with: {0}", filepath);

				objl::Loader objLoader;
				if(objLoader.LoadFile(filepath)){
					MeshInfo meshInfo;

					const size_t verticesSize = objLoader.LoadedVertices.size();
					meshInfo.verticies.reserve(verticesSize);
					meshInfo.texCoords.reserve(verticesSize);
					meshInfo.normals.reserve(verticesSize);

					for(const auto& vertex : objLoader.LoadedVertices){
						meshInfo.verticies.emplace_back(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
						meshInfo.texCoords.emplace_back(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y);
						meshInfo.normals.emplace_back(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
					}

					meshInfo.indices = objLoader.LoadedIndices;

					const size_t vertexCount	= meshInfo.verticies.size();
					const size_t texCoordCount	= meshInfo.texCoords.size();
					const size_t normalCount	= meshInfo.normals.size();

					meshInfo.vertexData.reserve((vertexCount * 3) + (texCoordCount * 2) + (normalCount * 3));

					for(int32 i = 0; i < vertexCount; ++i){
						if(vertexCount > 0){
							meshInfo.vertexData.emplace_back(meshInfo.verticies[i].x);
							meshInfo.vertexData.emplace_back(meshInfo.verticies[i].y);
							meshInfo.vertexData.emplace_back(meshInfo.verticies[i].z);
						}

						if(texCoordCount > 0){
							meshInfo.vertexData.emplace_back(meshInfo.texCoords[i].x);
							meshInfo.vertexData.emplace_back(meshInfo.texCoords[i].y);
						}

						if(normalCount > 0){
							meshInfo.vertexData.emplace_back(meshInfo.normals[i].x);
							meshInfo.vertexData.emplace_back(meshInfo.normals[i].y);
							meshInfo.vertexData.emplace_back(meshInfo.normals[i].z);
						}
					}

					loadedMeshes[filepath] = meshInfo;

					return loadedMeshes[filepath];
				} else{
					CLV_LOG_ERROR("Could not load file: {0}", filepath);

					return emptyMeshInfo;
				}
			}
		}
	}
}
