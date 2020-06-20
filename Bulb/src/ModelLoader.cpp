#include "Bulb/ModelLoader.hpp"

#include "Bulb/Rendering/Renderables/Mesh.hpp"

#include <Clove/Graphics/Texture.hpp>
#include <Clove/Graphics/VertexLayout.hpp>
#include <Clove/Platform/Window.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace clv;

namespace blb::ModelLoader {
	static mth::mat4f convertToGarlicMatrix(const aiMatrix4x4& aiMat){
        mth::mat4f garlicMat;

        garlicMat[0][0] = aiMat.a1;
        garlicMat[1][0] = aiMat.b1;
        garlicMat[2][0] = aiMat.c1;
        garlicMat[3][0] = aiMat.d1;

        garlicMat[0][1] = aiMat.a2;
        garlicMat[1][1] = aiMat.b2;
        garlicMat[2][1] = aiMat.c2;
        garlicMat[3][1] = aiMat.d2;

        garlicMat[0][2] = aiMat.a3;
        garlicMat[1][2] = aiMat.b3;
        garlicMat[2][2] = aiMat.c3;
        garlicMat[3][2] = aiMat.d3;

        garlicMat[0][3] = aiMat.a4;
        garlicMat[1][3] = aiMat.b4;
        garlicMat[2][3] = aiMat.c4;
        garlicMat[3][3] = aiMat.d4;

        return garlicMat;
	}

	static std::shared_ptr<gfx::Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory) {
		std::shared_ptr<gfx::Texture> texture;

		//TODO: Support multiple textures of the same type
		if(material->GetTextureCount(type) > 0) {
			aiString path;
			material->GetTexture(type, 0, &path);

			gfx::TextureDescriptor descriptor{};
			texture = graphicsFactory->createTexture(descriptor, path.C_Str());
		}

		return texture;
	}

	static std::shared_ptr<rnd::Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory) {
		gfx::VertexLayout layout;
		if(mesh->HasPositions()) {
			layout.add(gfx::VertexElementType::position3D);
		}
		if(mesh->HasNormals()) {
			layout.add(gfx::VertexElementType::normal);
		}
		//if(mesh->HasTextureCoords(0)) {
		layout.add(gfx::VertexElementType::texture2D);
		//}
		//Skipping colours for now
		/*if(mesh->HasVertexColors(0)) {
			layout.add(gfx::VertexElementType::colour3D);
		}*/

		gfx::VertexBufferData vertexBufferData{ layout };
		std::vector<uint32_t> indices;
		std::shared_ptr<rnd::Material> meshMaterial = std::make_shared<rnd::Material>(graphicsFactory);

		const size_t vertexCount = mesh->mNumVertices;
		vertexBufferData.resize(vertexCount);

		for(size_t i = 0; i < vertexCount; ++i) {
			if(mesh->HasPositions()) {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::position3D>() = {
					mesh->mVertices[i].x,
					mesh->mVertices[i].y,
					mesh->mVertices[i].z
				};
			}
			if(mesh->HasNormals()) {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::normal>() = {
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
				};
			}
			if(mesh->HasTextureCoords(0)) {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::texture2D>() = {
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			} else {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::texture2D>() = {
					0.0f,
					0.0f
				};
			}
		}

		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			for(size_t j = 0; j < face.mNumIndices; ++j) {
				indices.emplace_back(face.mIndices[j]);
			}
		}

		if(mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuseTexture = loadMaterialTexture(material, aiTextureType_DIFFUSE, graphicsFactory);
			auto specularTexture = loadMaterialTexture(material, aiTextureType_SPECULAR, graphicsFactory);

			if(diffuseTexture) {
				meshMaterial->setAlbedoTexture(std::move(diffuseTexture));
			}
			if(specularTexture) {
				meshMaterial->setSpecularTexture(std::move(specularTexture));
			}
		}

		return std::make_shared<rnd::Mesh>(vertexBufferData, indices, meshMaterial->createInstance());
	}

	rnd::Model loadModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory) {
		CLV_PROFILE_FUNCTION();

		std::vector<std::shared_ptr<rnd::Mesh>> meshes;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(modelFilePath.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
		if(scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Assimp Error: {0}", importer.GetErrorString());
			return { meshes };
		}

		for(size_t i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            meshes.emplace_back(processMesh(mesh, scene, graphicsFactory));
        }

		return { meshes };
	}

	rnd::SkeletalMesh loadAnimatedModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory){
        CLV_PROFILE_FUNCTION();

        std::vector<std::shared_ptr<rnd::Mesh>> meshes;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelFilePath.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
        if(scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Assimp Error: {0}", importer.GetErrorString());
            return { meshes };
        }

		for(size_t i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            meshes.emplace_back(processMesh(mesh, scene, graphicsFactory));
        }

		rnd::SkeletalMesh animatedModel{ meshes };

		//TODO: Support multiple skeletons?
		rnd::Skeleton skeleton;

		//Go through bones
        for(size_t i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            if(mesh->mNumBones <= 0) {
                continue;
			}

            skeleton.joints.resize(mesh->mNumBones);
            for(size_t i = 0; i < mesh->mNumBones; i++) {
                aiBone* bone = mesh->mBones[i];

				//TODO: Parent
				skeleton.joints[i].inverseBindPose = convertToGarlicMatrix(bone->mOffsetMatrix);

				//TODO: Get weights, have to index back into the vertex buffer
			}

			//Just doing one skeleton for now
			break;
        }

        return animatedModel;
	}
}