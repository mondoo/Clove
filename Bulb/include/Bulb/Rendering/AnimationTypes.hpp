#pragma once

namespace blb::rnd {
    struct Joint {
        //TODO: it looks like this will be calculated on inport?
        clv::mth::mat4f inverseBindPose;
        uint8_t parentIndex;
    };

    struct Skeleton {
        std::vector<Joint> joints;
    };

    //The position that the joint will be in when this pose is applied to it (in joint space relative to parent)
    struct JointPose {
        clv::mth::quatf rotation;
        clv::mth::vec4f position;
        clv::mth::vec4f scale;
    };

    //The pose for an entire skeleton, a keyframe in the animation
    struct AnimationPose {
        float timeStamp;
        std::vector<JointPose> poses; //one for each joint
    };

    //An entire animation (like walk or run)
    struct AnimationClip {
        Skeleton* skeleton;
        float duration;
        std::vector<AnimationPose> poses; //all the poses for this clip
    };
}