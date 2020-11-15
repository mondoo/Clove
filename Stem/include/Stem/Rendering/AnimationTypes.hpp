#pragma once

namespace garlic::inline stem {
    using JointIndexType = uint8_t;

    struct Joint {
        std::string name;
        clv::mth::mat4f inverseBindPose;//Bm->j
        std::optional<JointIndexType> parentIndex;
    };

    struct Skeleton {
        std::vector<Joint> joints;
    };

    //The position that the joint will be in when this pose is applied to it (in joint space relative to parent)
    struct JointPose {
        clv::mth::quatf rotation;
        clv::mth::vec3f position;
        clv::mth::vec3f scale;
    };

    //The pose for an entire skeleton, a keyframe in the animation
    struct AnimationPose {
        float timeStamp;
        std::vector<JointPose> poses;//one for each joint
    };

    //An entire animation (like walk or run)
    struct AnimationClip {
        Skeleton *skeleton;
        float duration;
        std::vector<AnimationPose> poses;//all the poses for this clip
    };
}