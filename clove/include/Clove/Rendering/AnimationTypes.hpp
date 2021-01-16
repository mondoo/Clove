#pragma once

#include <Clove/Maths/Matrix.hpp>
#include <Clove/Maths/Quaternion.hpp>
#include <Clove/Maths/Vector.hpp>
#include <cinttypes>
#include <optional>
#include <string>
#include <vector>

namespace garlic::clove {
    using JointIndexType = uint8_t;

    struct Joint {
        std::string name;
        mat4f inverseBindPose;//Bm->j
        std::optional<JointIndexType> parentIndex;
    };

    struct Skeleton {
        std::vector<Joint> joints;
    };

    //The position that the joint will be in when this pose is applied to it (in joint space relative to parent)
    struct JointPose {
        quatf rotation{ vec3f{ 0.0f } };
        vec3f position{ 0.0f };
        vec3f scale{ 1.0f };
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