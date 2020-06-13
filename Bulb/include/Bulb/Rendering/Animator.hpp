#pragma once

//Temp, need the join pose
#include "Bulb/Rendering/AnimationTypes.hpp"

//TODO: Move to cpp
namespace blb::rnd {
    static std::vector<JointPose> lerpJointPoses(const AnimationPose& posesA, const AnimationPose& posesB, float time) {
        //TODO:
    }

    static std::vector<clv::mth::mat4f> calculateCurrentJointToWorlMatrices(const std::vector<JointPose>& poses, const Skeleton* skeleton){
        //TODO
    }
}

namespace blb::rnd {
    //TODO: Returns an array of matrices of where a joint should be, so index 4 of the array is the position for joint 4 etc.
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

            //Lerp between them
            const float timeBetweenPoses = nextPose.timeStamp - prevPose.timeStamp;
            const float timeFromPrevPose = currentTime - prevPose.timeStamp;
            const float normTime         = timeFromPrevPose / timeBetweenPoses;

            auto currentAnimPose = lerpJointPoses(prevPose, nextPose, normTime);

            //Get the current model transform of the pose (Cj->m)
            auto currentJointToModel = calculateCurrentJointToWorlMatrices(currentAnimPose, currentClip.skeleton);

            //Calculate skinning matrix K = Bm->j * Cj->m
            std::vector<clv::mth::mat4f> skinningMatrix(currentJointToModel.size());
            for(size_t i = 0; i < currentJointToModel.size(); ++i) {
                skinningMatrix[i] = currentClip.skeleton->joints[i].inverseBindPose * currentJointToModel[i];
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