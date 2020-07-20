#pragma once

#include "Bulb/Rendering/Renderables/Model.hpp"
#include "Bulb/Rendering/AnimationTypes.hpp"
#include "Bulb/Rendering/Animator.hpp"

namespace blb::rnd {
    class AnimatedModel : public Model {
        //VARIABLES
   // private:
    public:
        //Temp, owning the skeleton for now but when using submeshes it will make sense to have this stored somewhere else. Which also allow skeletons to be reused
        std::unique_ptr<Skeleton> skeleton;
        Animator animator; //TODO: not sure if this should live here
        std::vector<AnimationClip> animClips; //TODO: Should clips live on the animated model?

        //FUNCTIONS
    public:
        //TODO: Ctors
    };
}