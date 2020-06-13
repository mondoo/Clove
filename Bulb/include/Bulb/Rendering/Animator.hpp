#pragma once

//Temp:
#include "Bulb/Rendering/Renderables/SkeletalMesh.hpp"

namespace blb::rnd {
    struct AnimationPose {
        std::vector<JointPose> poses;
    };

    struct AnimationClip {
        Skeleton* skeleton;
        float duration;
        std::vector<AnimationPose> poses;
    };
}

namespace blb::rnd {
    class Animator {
    };
}