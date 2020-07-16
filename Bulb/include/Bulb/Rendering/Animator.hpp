#pragma once

//Temp, need the join pose
#include "Bulb/Rendering/AnimationTypes.hpp"
#include "Bulb/Rendering/RenderingConstants.hpp"

//TODO: Move to cpp
namespace blb::rnd {
    static std::vector<JointPose> lerpJointPoses(const AnimationPose& posesA, const AnimationPose& posesB, const float time) {
        const size_t jointCount = posesA.poses.size();

        std::vector<JointPose> lerpedPoses;
        lerpedPoses.resize(jointCount);

        for(size_t i = 0; i < jointCount; ++i) {
            const auto pos   = clv::mth::lerp(posesA.poses[i].position, posesB.poses[i].position, time);
            const auto rot   = clv::mth::slerp(posesA.poses[i].rotation, posesB.poses[i].rotation, time);
            const auto scale = clv::mth::lerp(posesA.poses[i].scale, posesB.poses[i].scale, time);

            lerpedPoses[i] = { rot, pos, scale };
        }

        return lerpedPoses;
    }

    static clv::mth::mat4f getJointToModelMatrix(const JointPose& pose, const std::optional<JointIndexType>& parentIndex, const std::vector<JointPose>& poses, const Skeleton* skeleton) {
        //TODO: A lot of recalculations here, perhaps we can make it so we don't revisit joints once they are done

        const clv::mth::mat4f translationMatrix = clv::mth::translate(clv::mth::mat4f(1.0f), pose.position);
        const clv::mth::mat4f rotationMatrix    = clv::mth::quaternionToMatrix4(pose.rotation);
        const clv::mth::mat4f scaleMatrix       = clv::mth::scale(clv::mth::mat4f(1.0f), pose.scale);

        const clv::mth::mat4f poseTransform = translationMatrix * rotationMatrix * scaleMatrix;

        if(parentIndex.has_value()) {
            const JointIndexType index                = parentIndex.value();
            const clv::mth::mat4f parentPoseTransform = getJointToModelMatrix(poses[index], skeleton->joints[index].parentIndex, poses, skeleton);

            return parentPoseTransform * poseTransform;
        } else {
            return poseTransform;
        }
    }

    static std::vector<clv::mth::mat4f> calculateCurrentJointToModelMatrices(const std::vector<JointPose>& poses, const Skeleton* skeleton) {
        const size_t size = poses.size();
        std::vector<clv::mth::mat4f> jointToModelMatrices(size);
        for(size_t i = 0; i < size; ++i) {
            jointToModelMatrices[i] = getJointToModelMatrix(poses[i], skeleton->joints[i].parentIndex, poses, skeleton);
        }
        return jointToModelMatrices;
    }
}

namespace blb::rnd {
    class Animator {
        //VARIABLES
        //private:
    public:
        float currentTime = 0.0f;
        AnimationClip* currentClip = nullptr;

        //FUNCTIONS
    public:
        //TODO: Ctors

        std::array<clv::mth::mat4f, MAX_JOINTS> update(const clv::utl::DeltaTime deltaTime) {
            if(currentClip == nullptr) {
                GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "{0}: Current clip is now set, could not create palet", CLV_FUNCTION_NAME);
                return {};
            }

            //TODO: single play (current will loop)
            currentTime = fmod(currentTime + deltaTime, currentClip->duration);

            //Get the poses either side of the animation time
            const auto& [prevPose, nextPose] = getPrevNextPose(currentTime);

            //Lerp between them to create the target pose
            const float timeBetweenPoses = nextPose.timeStamp - prevPose.timeStamp;
            const float timeFromPrevPose = currentTime - prevPose.timeStamp;
            const float normTime         = timeFromPrevPose / timeBetweenPoses;

            const auto currentAnimPose = lerpJointPoses(prevPose, nextPose, normTime);

            //Get the current joint to model matrix of the target pose (Cj->m)
            const auto currentJointToModel = calculateCurrentJointToModelMatrices(currentAnimPose, currentClip->skeleton);

            //Calculate skinning matrix K = Cj->m * Bm->j (right to left)
            std::array<clv::mth::mat4f, MAX_JOINTS> skinningMatrix;
            for(size_t i = 0; i < std::size(skinningMatrix); ++i) {
                if(i < std::size(currentJointToModel)) {
                    skinningMatrix[i] = currentJointToModel[i] * currentClip->skeleton->joints[i].inverseBindPose;
                } else {
                    skinningMatrix[i] = clv::mth::mat4f{ 1.0f };
                }
            }

            return skinningMatrix;
        }

    private:
        std::pair<const AnimationPose&, const AnimationPose&> getPrevNextPose(float animationTime){
            size_t prevIndex = 0;
            size_t nextIndex = 0;

            for(size_t i = 0; i < currentClip->poses.size(); ++i) {
                nextIndex = i;
                if(currentClip->poses[i].timeStamp > animationTime) {
                    break;
                }
                prevIndex = i;
            }

            return { currentClip->poses[prevIndex], currentClip->poses[nextIndex] };
        }
    };
}