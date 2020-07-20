#include "Bulb/Rendering/Renderables/AnimatedModel.hpp"

namespace blb::rnd {
    AnimatedModel::AnimatedModel(std::vector<std::shared_ptr<Mesh>> meshes, std::unique_ptr<Skeleton> skeleton, std::vector<AnimationClip> animClips)
        : StaticModel(std::move(meshes))
        , skeleton(std::move(skeleton))
        , animClips(std::move(animClips)) {
        if (std::size(this->animClips) == 0){
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Warning, "AnimatedModel initialised without any animation clips. Won't be able to play animations");
        } else {
            animator.setCurrentClip(&this->animClips[0]);
        }
    }

    AnimatedModel::AnimatedModel(AnimatedModel&& other) noexcept
        : StaticModel(other)
        , animator(std::move(other.animator))
        , skeleton(std::move(other.skeleton))
        , animClips(std::move(other.animClips)){
    }

    AnimatedModel& AnimatedModel::operator=(AnimatedModel&& other) noexcept{
        StaticModel::operator=(other);
        animator             = std::move(other.animator);
        skeleton             = std::move(other.skeleton);
        animClips            = std::move(other.animClips);

        return *this;
    }

    AnimatedModel::~AnimatedModel() = default;
    
    std::array<clv::mth::mat4f, MAX_JOINTS> AnimatedModel::update(const clv::utl::DeltaTime deltaTime) {
        return animator.update(deltaTime);
    }
}