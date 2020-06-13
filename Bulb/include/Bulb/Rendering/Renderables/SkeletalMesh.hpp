#pragma once

namespace blb::rnd {
    struct Joint {
        clv::mth::mat4f inverseBindPose;
        uint8_t parentIndex;
    };

    struct Skeleton {
        std::vector<Joint> joints;
    };
}

namespace blb::rnd {
    class SkeletalMesh {
    };
}