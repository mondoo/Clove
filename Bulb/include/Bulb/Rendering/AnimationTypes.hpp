#pragma once

namespace blb::rnd {
    //Skeleton Data
    struct Joint {
        clv::mth::mat4f inverseBindPose;
        uint8_t parentIndex;
    };

    struct Skeleton {
        std::vector<Joint> joints;
    };

    struct JointPose {
        clv::mth::quatf rotation;
        clv::mth::vec4f position;
        clv::mth::vec4f scale;
    };

    struct AnimationPose {
        std::vector<JointPose> poses;
    };

    struct AnimationClip {
        Skeleton* skeleton;
        float duration;
        std::vector<AnimationPose> poses;
    };
}