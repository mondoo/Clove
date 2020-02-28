#include "Tunic/ModelLoader.hpp"

#include "Tunic/Rendering/Renderables/Mesh.hpp"

#include <Clove/Graphics/Core/VertexLayout.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace tnc::ModelLoader {
	//TEMP: Returning a vector of strings for now
	static std::vector<std::string> loadMaterialTextures(aiMaterial* material, aiTextureType type) {
		std::vector<std::string> paths;
		for(size_t i = 0; i < material->GetTextureCount(type); ++i) {
			aiString str;
			material->GetTexture(type, i, &str);
			paths.emplace_back(str.C_Str());
		}

		return paths;
	}

	static std::shared_ptr<rnd::Mesh> processMesh(aiMesh* mesh, const aiScene* scene) {
		clv::gfx::VertexBufferData vertexBufferData{
			{} //TODO: Proper layout
		};
		std::vector<uint32_t> indices;
		std::shared_ptr<rnd::Material> material;

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			//TODO: Vertices
		}

		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			for(size_t j = 0; j < face.mNumIndices; ++j) {
				indices.emplace_back(face.mIndices[j]);
			}
		}

		if(mesh->mMaterialIndex >= 0) {
			std::vector<std::string> diffuseTextures = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
			std::vector<std::string> specularTextures = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR);
			//TODO: Process textures
		}

		return std::make_shared<rnd::Mesh>(vertexBufferData, indices, material->createInstance());
	}
	
	//TODO: Needs to take in a Model to add to
	static void processNode(aiNode* node, const aiScene* scene) {
		for(size_t i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			//TODO: Push mesh back
		}

		for(size_t i = 0; i < node->mNumChildren; ++i) {
			processNode(node->mChildren[i], scene);
		}
	}

	Model loadModel(std::string_view filePath) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
		if(scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr) {
			CLV_LOG_ERROR("Assimp Error: {0}", importer.GetErrorString());
			return {};
		}

		processNode(scene->mRootNode, scene);

		//TEMP: returning nothing
		return {};
	}
}