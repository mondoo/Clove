#pragma once

#include "Bulb/Rendering/AnimationTypes.hpp"
#include "Bulb/Rendering/Animator.hpp"
#include "Bulb/Rendering/Renderables/StaticModel.hpp"

namespace blb::rnd {
    /**
     * @brief Represents a collection of meshes that can be animated.
     */
    class AnimatedModel : public StaticModel {
        //VARIABLES
    private:
        Animator animator;

        std::unique_ptr<Skeleton> skeleton;
        std::vector<AnimationClip> animClips;

        //FUNCTIONS
    public:
        AnimatedModel() = delete;
        AnimatedModel(std::vector<std::shared_ptr<Mesh>> meshes, std::unique_ptr<Skeleton> skeleton, std::vector<AnimationClip> animClips);

        AnimatedModel(const AnimatedModel& other);
        AnimatedModel(AnimatedModel&& other) noexcept;

        AnimatedModel& operator=(const AnimatedModel& other);
        AnimatedModel& operator=(AnimatedModel&& other) noexcept;

        ~AnimatedModel();

        /**
         * @brief Updates the internal animator.
         * @param deltaTime The time since the last frame
         * @returns The matrix palette for the skeleton for a given frame
         */
        std::array<clv::mth::mat4f, MAX_JOINTS> update(const clv::utl::DeltaTime deltaTime);
    };
}