#include "Clove/Systems/AudioSystem.hpp"

#include "Clove/Components/AudioListenerComponent.hpp"
#include "Clove/Components/AudioSourceComponent.hpp"
#include "Clove/Components/TransformComponent.hpp"

#include <Clove/ECS/World.hpp>
#include <Clove/Audio/AudioListener.hpp>
#include <Clove/Audio/AudioSource.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    AudioSystem::AudioSystem() = default;

    AudioSystem::AudioSystem(AudioSystem &&other) noexcept = default;

    AudioSystem &AudioSystem::operator=(AudioSystem &&other) noexcept = default;

    AudioSystem::~AudioSystem() = default;

    void AudioSystem::update(World &world, DeltaTime deltaTime) {
        CLV_PROFILE_FUNCTION();

        //Listener
        auto const listenerSet{ world.getComponentSets<TransformComponent, AudioListenerComponent>() };
        size_t const numListeners{ std::size(listenerSet) };
        GARLIC_ASSERT(numListeners <= 1, "Only one listener is allowed per world");
        if(numListeners > 0) {
            auto &&[transform, listener] = listenerSet[0];

            vec3f const prevPos{ listener->listener->getPosition() };
            vec3f const currPos{ transform->getPosition() };

            listener->listener->setPosition(currPos);
            listener->listener->setVelocity(currPos - prevPos);
        }

        //Source
        for(auto &&[transform, source] : world.getComponentSets<TransformComponent, AudioSourceComponent>()) {
            vec3f const prevPos{ source->source->getPosition() };
            vec3f const currPos{ transform->getPosition() };

            source->source->setPosition(currPos);
            source->source->setVelocity(currPos - prevPos);
        }
    }
}