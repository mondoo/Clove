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
        garlicMat[0][1] = aiMat.b1;
        garlicMat[0][2] = aiMat.c1;
        garlicMat[0][3] = aiMat.d1;

        garlicMat[1][0] = aiMat.a2;
        garlicMat[1][1] = aiMat.b2;
        garlicMat[1][2] = aiMat.c2;
        garlicMat[1][3] = aiMat.d2;

        garlicMat[2][0] = aiMat.a3;
        garlicMat[2][1] = aiMat.b3;
        garlicMat[2][2] = aiMat.c3;
        garlicMat[2][3] = aiMat.d3;

        garlicMat[3][0] = aiMat.a4;
        garlicMat[3][1] = aiMat.b4;
        garlicMat[3][2] = aiMat.c4;
        garlicMat[3][3] = aiMat.d4;

        return garlicMat;
	}

	static mth::vec3f convertToGarlicVec(const aiVector3D& aiVec){
        return { aiVec.x, aiVec.y, aiVec.z };
	}

	static mth::quatf convertToGarlicQuat(const aiQuaternion& aiQuat){
        return { aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z };
	}

	static void buildNodeNameMap(std::unordered_map<std::string_view, aiNode*>& map, aiNode* rootNode){
        map[rootNode->mName.C_Str()] = rootNode;
        for(size_t i = 0; i < rootNode->mNumChildren; ++i) {
            buildNodeNameMap(map, rootNode->mChildren[i]);
		}
	}

	static std::optional<rnd::JointIndexType> getJointParentId(const rnd::Skeleton& skeleton, std::string_view jointName, std::unordered_map<std::string_view, aiNode*>& nodeNameMap) {
        aiNode* jointNode = nodeNameMap[jointName];
        if(jointNode->mParent == nullptr) {
            return {};
        }

		aiString* parentName = &jointNode->mParent->mName;
        for(size_t i = 0; i < skeleton.joints.size(); ++i) {
            if(skeleton.joints[i].name == parentName->C_Str()) {
                return i;
			}
		}
		GARLIC_LOG(garlicLogContext, Log::Level::Debug, "{0}: Joint \"{1}\" has a parent but it couldn't be found in the skeleton", CLV_FUNCTION_NAME, jointName);

		return {};
	}

	static rnd::JointIndexType getJointIndex(const rnd::Skeleton& skeleton, std::string_view jointName){
        for(size_t i = 0; i < skeleton.joints.size(); ++i) {
            if(skeleton.joints[i].name == jointName) {
                return i;
            }
        }
		//TODO: Error
        return std::numeric_limits<rnd::JointIndexType>::max();
	}

    static size_t getPreviousIndex(const rnd::AnimationClip& clip, float frame, float framesPerSecond, rnd::JointIndexType jointIndex, std::map<float, size_t>& framePoseIndexMap, std::map<float, std::vector<rnd::JointIndexType>>& missingPoseMap) {
        const size_t currIndex = framePoseIndexMap[frame];
        for(int i = currIndex - 1; i >= 0; --i) {
            const float poseFrame   = clip.poses[i].timeStamp * framesPerSecond;//Get the time of the pose before this one to check if it's not a missing pose
            const auto missingPoses = missingPoseMap[poseFrame];

            const bool isValidTimeStamp = framePoseIndexMap.find(poseFrame) != framePoseIndexMap.end();
            const bool isMissingPose    = std::find(missingPoses.begin(), missingPoses.end(), jointIndex) != missingPoses.end();
            if(isValidTimeStamp && !isMissingPose) {
                return i;
            }
        }
        
        return -1;
    }

    static size_t getNextIndex(const rnd::AnimationClip& clip, float frame, float framesPerSecond, rnd::JointIndexType jointIndex, std::map<float, size_t>& framePoseIndexMap, std::map<float, std::vector<rnd::JointIndexType>>& missingPoseMap) {
        const size_t currIndex = framePoseIndexMap[frame];
        for(int i = currIndex + 1; i < clip.poses.size(); ++i) {
            const float poseFrame   = clip.poses[i].timeStamp * framesPerSecond;//Get the time of the pose after this one to check if it's not a missing pose
            const auto missingPoses = missingPoseMap[poseFrame];

            const bool isValidTimeStamp = framePoseIndexMap.find(poseFrame) != framePoseIndexMap.end();
            const bool isMissingPose    = std::find(missingPoses.begin(), missingPoses.end(), jointIndex) != missingPoses.end();
            if(isValidTimeStamp && !isMissingPose) {
                return i;
            }
        }

        return -1;
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

	enum class MeshType{
		Default,
		Animated
	};
	static std::shared_ptr<rnd::Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory, const MeshType meshType) {
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
        if(meshType == MeshType::Animated) {
            layout.add(gfx::VertexElementType::jointIds);
            layout.add(gfx::VertexElementType::weights);
		}
		//Skipping colours for now
		/*if(mesh->HasVertexColors(0)) {
			layout.add(gfx::VertexElementType::colour3D);
		}*/

		gfx::VertexBufferData vertexBufferData{ layout };
		std::vector<uint32_t> indices;
		std::shared_ptr<rnd::Material> meshMaterial = std::make_shared<rnd::Material>(graphicsFactory);

		const size_t vertexCount = mesh->mNumVertices;
		vertexBufferData.resize(vertexCount);

		//Build the map of jointIds + weights for each vertex
        std::unordered_map<size_t, std::vector<std::pair<rnd::JointIndexType, float>>> vertWeightPairs;
		if(meshType == MeshType::Animated) {
            for(rnd::JointIndexType i = 0; i < mesh->mNumBones; ++i) {
                aiBone* bone = mesh->mBones[i];
                for(size_t j = 0; j < bone->mNumWeights; ++j) {
                    const aiVertexWeight& vertexWeight = bone->mWeights[j];
                    vertWeightPairs[vertexWeight.mVertexId].emplace_back(i, vertexWeight.mWeight);
                }
            }
        }

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
            if(meshType == MeshType::Animated) {
                mth::vec4i& jointIds = vertexBufferData[i].getAttribute<gfx::VertexElementType::jointIds>();
                mth::vec4f& weights  = vertexBufferData[i].getAttribute<gfx::VertexElementType::weights>();
                const std::vector<std::pair<rnd::JointIndexType, float>>& weightPairs = vertWeightPairs[i];

                for(size_t j = 0; j < 4; ++j){
                    if(j < weightPairs.size()) {
                        jointIds[j] = weightPairs[j].first;
                        weights[j]  = weightPairs[j].second;
                    } else {
                        jointIds[j] = 0;
                        weights[j]  = 0.0f;
					}
				}
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

	rnd::StaticModel loadStaticModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory) {
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
            meshes.emplace_back(processMesh(mesh, scene, graphicsFactory, MeshType::Default));
        }

		return { meshes };
	}

	rnd::AnimatedModel loadAnimatedModel(std::string_view modelFilePath, const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory){
        CLV_PROFILE_FUNCTION();

        std::vector<std::shared_ptr<rnd::Mesh>> meshes;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelFilePath.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
        if(scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Assimp Error: {0}", importer.GetErrorString());
            return { meshes, nullptr, {} };
        }

		//TODO: Support multiple skeletons?
		std::unique_ptr<rnd::Skeleton> skeleton = std::make_unique<rnd::Skeleton>(); //TODO: Made into a unique ptr to avoid errors from std::move. Perhaps we should put this in an allocator?

		//Build scene map
        std::unordered_map<std::string_view, aiNode*> nodeNameMap;
        buildNodeNameMap(nodeNameMap, scene->mRootNode);

		//Build skeleton
        bool skeletonSet = false;
        for(size_t i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            meshes.emplace_back(processMesh(mesh, scene, graphicsFactory, MeshType::Animated));

            if(mesh->mNumBones <= 0 || skeletonSet) {
                continue;
			}

            skeleton->joints.resize(mesh->mNumBones);
            for(size_t i = 0; i < mesh->mNumBones; i++) {
                aiBone* bone                        = mesh->mBones[i];
                skeleton->joints[i].name            = bone->mName.C_Str();
                skeleton->joints[i].inverseBindPose = convertToGarlicMatrix(bone->mOffsetMatrix);
            }

			//Only doing one sekelton for now
            skeletonSet = true;
        }

		//Set parents
        for(auto& joint : skeleton->joints) {
            joint.parentIndex = getJointParentId(*skeleton, joint.name, nodeNameMap);
		}

		//Load animations
        std::vector<rnd::AnimationClip> animationClips(scene->mNumAnimations);
        for(size_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex) {
            aiAnimation* animation = scene->mAnimations[animIndex];
            rnd::AnimationClip& animClip = animationClips[animIndex];
			
			animClip.skeleton = skeleton.get();
            animClip.duration = animation->mDuration / animation->mTicksPerSecond; //Our clip is in seconds where as the animation is in frames

            //Get all the key frame times for every possible channel
            std::set<float> frames;
            for(size_t channelIndex = 0; channelIndex < animation->mNumChannels; ++channelIndex) {
                aiNodeAnim* channel = animation->mChannels[channelIndex];
                for(size_t key = 0; key < channel->mNumPositionKeys; ++key) {
                    frames.emplace(channel->mPositionKeys[key].mTime);
                }
                for(size_t key = 0; key < channel->mNumRotationKeys; ++key) {
                    frames.emplace(channel->mRotationKeys[key].mTime);
                }
                for(size_t key = 0; key < channel->mNumScalingKeys; ++key) {
                    frames.emplace(channel->mScalingKeys[key].mTime);
                }
            }

			std::map<float, std::vector<rnd::JointIndexType>> missingPositions;
            std::map<float, std::vector<rnd::JointIndexType>> missingRotations;
            std::map<float, std::vector<rnd::JointIndexType>> missingScale;

            std::map<float, size_t> framePoseIndexMap;

			//Get each channel's pose at each time
            for(float frame : frames) {
                rnd::AnimationPose animPose{};
                animPose.timeStamp = frame / animation->mTicksPerSecond;
                animPose.poses.resize(std::size(skeleton->joints));

				for(size_t channelIndex = 0; channelIndex < animation->mNumChannels; ++channelIndex) {
                    aiNodeAnim* channel = animation->mChannels[channelIndex];

                    //TODO: The 'Armature' represents the root motion (I think). If so then this should contribute to the model and not the animation
                    if(strcmp(channel->mNodeName.C_Str(), "Armature") == 0) {
                        continue;
                    }

                    const rnd::JointIndexType jointIndex = getJointIndex(*skeleton, channel->mNodeName.C_Str());
                    rnd::JointPose& jointPose            = animPose.poses[jointIndex];
                    
					bool positionFound = false;
                    bool rotationFound = false;
                    bool scaleFound    = false;

					for(size_t key = 0; key < channel->mNumPositionKeys; ++key) {
                        if(channel->mPositionKeys[key].mTime == frame) {
                            jointPose.position = convertToGarlicVec(channel->mPositionKeys[key].mValue);
                            positionFound      = true;
                            break;
                        }
                    }
                    for(size_t key = 0; key < channel->mNumRotationKeys; ++key) {
                        if(channel->mRotationKeys[key].mTime == frame) {
                            jointPose.rotation = convertToGarlicQuat(channel->mRotationKeys[key].mValue);
                            rotationFound      = true;
                            break;
                        }
                    }
                    for(size_t key = 0; key < channel->mNumScalingKeys; ++key) {
                        if(channel->mScalingKeys[key].mTime == frame) {
                            jointPose.scale = convertToGarlicVec(channel->mScalingKeys[key].mValue);
                            scaleFound      = true;
                            break;
                        }
                    }

                    //Because the Animator expects every AnimationPose to have a pose for every joint
                    //we need to keep track of all the missing elements so we can interpolate between them later
                    //TODO: To simplify this we can refactor the animator to handle a pose not having an entry for every joints
                    if(!positionFound) {
                        missingPositions[frame].push_back(jointIndex);
                    }
                    if(!rotationFound) {
                        missingRotations[frame].push_back(jointIndex);
                    }
                    if(!scaleFound) {
                        missingScale[frame].push_back(jointIndex);
                    }
                }

				animClip.poses.emplace_back(std::move(animPose));
                framePoseIndexMap[frame] = std::size(animClip.poses) - 1;
			}

            const auto retrieveJointPoses = [&](float time, rnd::JointIndexType jointIndex, const rnd::AnimationPose& currAnimPose, std::map<float, std::vector<rnd::JointIndexType>>& missingElementMap) {
                struct LerpData{
                    const float lerpTime;
                    const rnd::JointPose& prevPose;
                    const rnd::JointPose& nextPose;
                };

                const size_t prevPoseIndex = getPreviousIndex(animClip, time, animation->mTicksPerSecond, jointIndex, framePoseIndexMap, missingElementMap);
                const size_t nextPoseIndex = getNextIndex(animClip, time, animation->mTicksPerSecond, jointIndex, framePoseIndexMap, missingElementMap);

                const rnd::AnimationPose& prevAnimPose = animClip.poses[prevPoseIndex];
                const rnd::AnimationPose& nextAnimPose = animClip.poses[nextPoseIndex];

                const float timeBetweenPoses = nextAnimPose.timeStamp - prevAnimPose.timeStamp;
                const float timeFromPrevPose = currAnimPose.timeStamp - prevAnimPose.timeStamp;
                const float normTime         = timeFromPrevPose / timeBetweenPoses;

                return LerpData{
                    normTime,
                    prevAnimPose.poses[jointIndex],
                    nextAnimPose.poses[jointIndex],
                };
            };

			//Interpolate missing keyframes
            for(auto& [time, jointIndices] : missingPositions) {
                const size_t currPoseIndex       = framePoseIndexMap[time];
                rnd::AnimationPose& currAnimPose = animClip.poses[currPoseIndex];

                for(rnd::JointIndexType jointIndex : jointIndices) {
                    const auto lerpData = retrieveJointPoses(time, jointIndex, currAnimPose, missingPositions);

                    rnd::JointPose& pose = currAnimPose.poses[jointIndex];
                    pose.position        = mth::lerp(lerpData.prevPose.position, lerpData.nextPose.position, lerpData.lerpTime);
                }
            }
            for(auto& [time, jointIndices] : missingRotations) {
                const size_t currPoseIndex       = framePoseIndexMap[time];
                rnd::AnimationPose& currAnimPose = animClip.poses[currPoseIndex];

                for(rnd::JointIndexType jointIndex : jointIndices) {
                    const auto lerpData = retrieveJointPoses(time, jointIndex, currAnimPose, missingRotations);

                    rnd::JointPose& pose = currAnimPose.poses[jointIndex];
                    pose.rotation        = mth::slerp(lerpData.prevPose.rotation, lerpData.nextPose.rotation, lerpData.lerpTime);
                }
            }
            for(auto& [time, jointIndices] : missingScale) {
                const size_t currPoseIndex       = framePoseIndexMap[time];
                rnd::AnimationPose& currAnimPose = animClip.poses[currPoseIndex];

                for(rnd::JointIndexType jointIndex : jointIndices) {
                    const auto lerpData = retrieveJointPoses(time, jointIndex, currAnimPose, missingScale);

                    rnd::JointPose& pose = currAnimPose.poses[jointIndex];
                    pose.scale           = mth::lerp(lerpData.prevPose.scale, lerpData.nextPose.scale, lerpData.lerpTime);
                }
            }
		}

		rnd::AnimatedModel animatedModel{ meshes, std::move(skeleton), std::move(animationClips) };
        return animatedModel;
	}
}