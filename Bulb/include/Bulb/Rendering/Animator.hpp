#pragma once

//Temp, need the join pose
#include "Bulb/Rendering/AnimationTypes.hpp"

//TODO: Move to cpp
namespace blb::rnd {
    static std::vector<JointPose> lerpJointPoses(const AnimationPose& posesA, const AnimationPose& posesB, float time) {
        //TODO:
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
    }
}

namespace blb::rnd {
    class Animator {
        //VARIABLES
        //private:
    public:
        float currentTime = 0.0f;
        AnimationClip currentClip;

        //FUNCTIONS
    public:
        //TODO: Ctors

        std::vector<clv::mth::mat4f> update(const clv::utl::DeltaTime deltaTime) {
            //TODO: single play (current will loop)
            currentTime = fmod(currentTime += deltaTime, currentClip.duration);

            //Get the poses either side of the animation
            auto& nextPose = getNextPose(currentTime);
            auto& prevPose = getPrevPose(currentTime);

            //Lerp between them to create the target pose
            const float timeBetweenPoses = nextPose.timeStamp - prevPose.timeStamp;
            const float timeFromPrevPose = currentTime - prevPose.timeStamp;
            const float normTime         = timeFromPrevPose / timeBetweenPoses;

            auto currentAnimPose = lerpJointPoses(prevPose, nextPose, normTime);

            //Get the current joint to model matrix of the target pose (Cj->m)
            auto currentJointToModel = calculateCurrentJointToModelMatrices(currentAnimPose, currentClip.skeleton);

            //Calculate skinning matrix K = Bm->j * Cj->m
            std::vector<clv::mth::mat4f> skinningMatrix(currentJointToModel.size());
            for(size_t i = 0; i < currentJointToModel.size(); ++i) {
                skinningMatrix[i] = currentJointToModel[i] * currentClip.skeleton->joints[i].inverseBindPose ;
            }

            return skinningMatrix;
        }

    private:
        const AnimationPose& getNextPose(float animationTime) {
            //TODO
        }

        const AnimationPose& getPrevPose(float animationtime) {
            //TODO
        }
    };
}