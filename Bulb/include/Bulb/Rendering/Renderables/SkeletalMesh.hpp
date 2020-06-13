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

    //Pose Data
    struct JointPose {
        clv::mth::quatf rotation;
        clv::mth::vec4f position;
        clv::mth::vec4f scale;
    };

    struct SkeletonPose {
        Skeleton* skeleton;
        //One item for every joint in the skeleton
        std::vector<JointPose> localPoses;
        std::vector<clv::mth::mat4f> globalPoses;
    };
}

namespace blb::rnd {
    class SkeletalMesh {
    };
}