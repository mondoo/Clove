#include "Bulb/ECS/Systems/AudioSystem.hpp"

#include "Bulb/ECS/Components/AudioListenerComponent.hpp"
#include "Bulb/ECS/Components/AudioSourceComponent.hpp"
#include "Bulb/ECS/Components/TransformComponent.hpp"
#include "Bulb/ECS/World.hpp"

#include <Clove/Audio/AudioListener.hpp>
#include <Clove/Audio/AudioSource.hpp>

using namespace clv;

namespace blb::ecs {
    AudioSystem::AudioSystem() = default;

    AudioSystem::AudioSystem(AudioSystem&& other) noexcept = default;

    AudioSystem& AudioSystem::operator=(AudioSystem&& other) noexcept = default;

    AudioSystem::~AudioSystem() = default;

    void AudioSystem::update(World& world, utl::DeltaTime deltaTime) {
        CLV_PROFILE_FUNCTION();

        //Listener
        const auto listenerSet = world.getComponentSets<TransformComponent, AudioListenerComponent>();
        const size_t numListeners  = std::size(listenerSet);
        GARLIC_ASSERT(numListeners <= 1, "Only one listener is allowed per world");
        if(numListeners > 0){
            auto&& [transform, listener] = listenerSet[0];

            const mth::vec3f prevPos = listener->listener->getPosition();
            const mth::vec3f currPos = transform->getPosition();

            listener->listener->setPosition(currPos);
            listener->listener->setVelocity(currPos - prevPos);
        }

        //Source
        for(auto&& [transform, source] : world.getComponentSets<TransformComponent, AudioSourceComponent>()) {
            const mth::vec3f prevPos = source->source->getPosition();
            const mth::vec3f currPos = transform->getPosition();

            source->source->setPosition(currPos);
            source->source->setVelocity(currPos - prevPos);
        }
    }
}