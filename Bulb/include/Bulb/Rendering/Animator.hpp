#pragma once

//Temp, need the join pose
#include "Bulb/Rendering/AnimationTypes.hpp"

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

        //Returns the model space transform for every bone (what will be uploaded into the shader)
        std::vector<clv::mth::mat4f> update(const clv::utl::DeltaTime deltaTime) {
            //TODO: single play (current will loop)
            currentTime = fmod(currentTime += deltaTime, currentClip.duration);

            //Get previous AnimationPose
            //Get next AnimationPose
            //Get normalised time between AnimationPoses
            //Lerp the JointPoses for them
            //Return an array of matrices for every Joint calculated from the lerped JointPose, in correct order
        }
    };
}