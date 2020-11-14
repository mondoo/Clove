#include "Bulb/Rendering/Renderables/AnimatedModel.hpp"

#include <Root/Log/Log.hpp>

namespace blb::rnd {
    AnimatedModel::AnimatedModel(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Material> material, std::unique_ptr<Skeleton> skeleton, std::vector<AnimationClip> animClips)
        : StaticModel(std::move(meshes), std::move(material))
        , skeleton(std::move(skeleton))
        , animClips(std::move(animClips)) {
        if(std::size(this->animClips) == 0) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, "AnimatedModel initialised without any animation clips. Won't be able to play animations");
        } else {
            animator.setCurrentClip(&this->animClips[0]);
        }
    }

    AnimatedModel::AnimatedModel(AnimatedModel const &other)
        : StaticModel(other)
        , skeleton(std::make_unique<Skeleton>(*other.skeleton))
        , animClips(other.animClips) {
        for(auto &clip : animClips) {
            clip.skeleton = skeleton.get();
        }

        if(std::size(animClips) == 0) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, "AnimatedModel initialised without any animation clips. Won't be able to play animations");
        } else {
            animator.setCurrentClip(&animClips[0]);
        }
    }

    AnimatedModel::AnimatedModel(AnimatedModel &&other) noexcept
        : StaticModel(other)
        , animator(std::move(other.animator))
        , skeleton(std::move(other.skeleton))
        , animClips(std::move(other.animClips)) {
    }

    AnimatedModel &AnimatedModel::operator=(AnimatedModel const &other) {
        StaticModel::operator=(other);
        skeleton             = std::make_unique<Skeleton>(*other.skeleton);
        animClips            = other.animClips;

        for(auto &clip : animClips) {
            clip.skeleton = skeleton.get();
        }

        if(std::size(animClips) == 0) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, "AnimatedModel initialised without any animation clips. Won't be able to play animations");
        } else {
            animator.setCurrentClip(&animClips[0]);
        }

        return *this;
    }

    AnimatedModel &AnimatedModel::operator=(AnimatedModel &&other) noexcept {
        StaticModel::operator=(other);
        animator             = std::move(other.animator);
        skeleton             = std::move(other.skeleton);
        animClips            = std::move(other.animClips);

        return *this;
    }

    AnimatedModel::~AnimatedModel() = default;
}