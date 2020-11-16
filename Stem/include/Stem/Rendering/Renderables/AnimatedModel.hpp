#pragma once

#include "Stem/Rendering/AnimationTypes.hpp"
#include "Stem/Rendering/Animator.hpp"
#include "Stem/Rendering/Renderables/StaticModel.hpp"

namespace garlic::inline stem {
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
        AnimatedModel(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Material> material, std::unique_ptr<Skeleton> skeleton, std::vector<AnimationClip> animClips);

        AnimatedModel(AnimatedModel const &other);
        AnimatedModel(AnimatedModel &&other) noexcept;

        AnimatedModel &operator=(AnimatedModel const &other);
        AnimatedModel &operator=(AnimatedModel &&other) noexcept;

        ~AnimatedModel();

        /**
         * @brief Updates the internal animator.
         * @param deltaTime The time since the last frame
         * @returns The matrix palette for the skeleton for a given frame
         */
        inline std::array<clv::mth::mat4f, MAX_JOINTS> update(clv::utl::DeltaTime const deltaTime);
    };
}

#include "AnimatedModel.inl"