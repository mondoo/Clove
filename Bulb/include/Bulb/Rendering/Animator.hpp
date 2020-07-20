#pragma once

#include "Bulb/Rendering/RenderingConstants.hpp"

namespace blb::rnd {
    class AnimationClip;
    class AnimationPose;
}

namespace blb::rnd {
    /**
     * @brief Drives the animation for an AnimatedModel.
     * @details Takes in an AnimationClip and uses that to calculate an AnimatedModel's skinning matrix palette for a given frame.
     */
    class Animator {
        //VARIABLES
    private:
        float currentTime          = 0.0f;
        AnimationClip* currentClip = nullptr;

        //FUNCTIONS
    public:
        Animator();
        
        Animator(const Animator& other);
        Animator(Animator&& other) noexcept;

        Animator& operator=(const Animator& other);
        Animator& operator=(Animator&& other) noexcept;

        ~Animator();

        /**
         * @brief Updates the Animator, generating a matrix palette.
         * @param deltaTime Time since last frame.
         * @returns The matrix palette for a given frame.
         */
        std::array<clv::mth::mat4f, MAX_JOINTS> update(const clv::utl::DeltaTime deltaTime);

        /**
         * @brief Sets the current clip the Animator will use
         * @param clip Pointer to the new clip
         */
        void setCurrentClip(AnimationClip* clip);

    private:
        std::pair<const AnimationPose&, const AnimationPose&> getPrevNextPose(float animationTime);
    };
}