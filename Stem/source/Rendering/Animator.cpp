#include "Stem/Rendering/Animator.hpp"

#include "Stem/Rendering/AnimationTypes.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

namespace garlic::inline stem {
    static std::vector<JointPose> lerpJointPoses(AnimationPose const &posesA, AnimationPose const &posesB, float const time) {
        size_t const jointCount{ posesA.poses.size() };

        std::vector<JointPose> lerpedPoses;
        lerpedPoses.resize(jointCount);

        for(size_t i = 0; i < jointCount; ++i) {
            auto const pos{ clv::mth::lerp(posesA.poses[i].position, posesB.poses[i].position, time) };
            auto const rot{ clv::mth::slerp(posesA.poses[i].rotation, posesB.poses[i].rotation, time) };
            auto const scale{ clv::mth::lerp(posesA.poses[i].scale, posesB.poses[i].scale, time) };

            lerpedPoses[i] = { rot, pos, scale };
        }

        return lerpedPoses;
    }

    static clv::mth::mat4f getJointToModelMatrix(JointPose const &pose, std::optional<JointIndexType> const &parentIndex, std::vector<JointPose> const &poses, Skeleton const *skeleton) {
        //TODO: A lot of recalculations here, perhaps we can make it so we don't revisit joints once they are done

        clv::mth::mat4f const translationMatrix{ clv::mth::translate(clv::mth::mat4f(1.0f), pose.position) };
        clv::mth::mat4f const rotationMatrix{ clv::mth::quaternionToMatrix4(pose.rotation) };
        clv::mth::mat4f const scaleMatrix{ clv::mth::scale(clv::mth::mat4f(1.0f), pose.scale) };

        clv::mth::mat4f const poseTransform{ translationMatrix * rotationMatrix * scaleMatrix };

        if(parentIndex.has_value()) {
            JointIndexType const index{ parentIndex.value() };
            clv::mth::mat4f const parentPoseTransform{ getJointToModelMatrix(poses[index], skeleton->joints[index].parentIndex, poses, skeleton) };

            return parentPoseTransform * poseTransform;
        } else {
            return poseTransform;
        }
    }

    static std::vector<clv::mth::mat4f> calculateCurrentJointToModelMatrices(std::vector<JointPose> const &poses, Skeleton const *skeleton) {
        size_t const size{ poses.size() };
        std::vector<clv::mth::mat4f> jointToModelMatrices(size);
        for(size_t i = 0; i < size; ++i) {
            jointToModelMatrices[i] = getJointToModelMatrix(poses[i], skeleton->joints[i].parentIndex, poses, skeleton);
        }
        return jointToModelMatrices;
    }

    Animator::Animator() = default;

    Animator::Animator(Animator const &other) = default;

    Animator::Animator(Animator &&other) noexcept = default;

    Animator &Animator::operator=(Animator const &other) = default;

    Animator &Animator::operator=(Animator &&other) noexcept = default;

    Animator::~Animator() = default;

    std::array<clv::mth::mat4f, MAX_JOINTS> Animator::update(clv::utl::DeltaTime const deltaTime) {
        CLV_PROFILE_FUNCTION();

        if(currentClip == nullptr) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "{0}: Current clip is not set, could not create palet", GARLIC_FUNCTION_NAME);
            return {};
        }

        //TODO: single play (current will loop)
        currentTime = fmod(currentTime + deltaTime, currentClip->duration);

        //Get the poses either side of the animation time
        auto const &[prevPose, nextPose] = getPrevNextPose(currentTime);

        //Lerp between them to create the target pose
        float const timeBetweenPoses{ nextPose.timeStamp - prevPose.timeStamp };
        float const timeFromPrevPose{ currentTime - prevPose.timeStamp };
        float const normTime{ timeFromPrevPose / timeBetweenPoses };

        auto const currentAnimPose{ lerpJointPoses(prevPose, nextPose, normTime) };

        //Get the current joint to model matrix of the target pose (Cj->m)
        auto const currentJointToModel{ calculateCurrentJointToModelMatrices(currentAnimPose, currentClip->skeleton) };

        //Calculate skinning matrix K = Cj->m * Bm->j (right to left)
        std::array<clv::mth::mat4f, MAX_JOINTS> skinningMatrix;
        for(size_t i = 0; i < std::size(skinningMatrix); ++i) {
            if(i < std::size(currentJointToModel)) {
                skinningMatrix[i] = currentJointToModel[i] * currentClip->skeleton->joints[i].inverseBindPose;
            } else {
                skinningMatrix[i] = clv::mth::mat4f{ 1.0f };
            }
        }

        return skinningMatrix;
    }

    void Animator::setCurrentClip(AnimationClip *clip) {
        currentClip = clip;
    }

    std::pair<AnimationPose const &, AnimationPose const &> Animator::getPrevNextPose(float animationTime) {
        size_t prevIndex{ 0 };
        size_t nextIndex{ 0 };

        for(size_t i = 0; i < currentClip->poses.size(); ++i) {
            nextIndex = i;
            if(currentClip->poses[i].timeStamp > animationTime) {
                break;
            }
            prevIndex = i;
        }

        return { currentClip->poses[prevIndex], currentClip->poses[nextIndex] };
    }
}