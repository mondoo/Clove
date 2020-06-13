#pragma once

#include "Bulb/Rendering/Renderables/Mesh.hpp"
#include "Bulb/Rendering/AnimationTypes.hpp"
#include "Bulb/Rendering/Animator.hpp"

//TODO: Support submeshes (probably convert this into a model)

namespace blb::rnd {
    class SkeletalMesh {
        //VARIABLES
   // private:
    public:
        //Temp, owning the skeleton for now but when using submeshes it will make sense to have this stored somewhere else. Which also allow skeletons to be reused
        Skeleton skeleton;
        Animator animator; //TODO: not sure if this should live here

        //TEMP: just storing a mesh now to reduce code duplication
        std::shared_ptr<Mesh> mesh;

        //FUNCTIONS
    public:
        //TODO: Ctors
    };
}