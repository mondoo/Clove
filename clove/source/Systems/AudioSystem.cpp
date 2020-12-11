#include "Clove/Systems/AudioSystem.hpp"

#include "Clove/Components/AudioListenerComponent.hpp"
#include "Clove/Components/AudioSourceComponent.hpp"
#include "Clove/Components/TransformComponent.hpp"

#include <Clove/Audio/AudioListener.hpp>
#include <Clove/Audio/AudioSource.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    AudioSystem::AudioSystem() = default;

    AudioSystem::AudioSystem(AudioSystem &&other) noexcept = default;

    AudioSystem &AudioSystem::operator=(AudioSystem &&other) noexcept = default;

    AudioSystem::~AudioSystem() = default;

    void AudioSystem::update(World &world) {
        CLOVE_PROFILE_FUNCTION();

        //Listener
        world.forEach([](TransformComponent const &transform, AudioListenerComponent &listener) {
            vec3f const prevPos{ listener.listener->getPosition() };
            vec3f const currPos{ transform.getPosition() };

            listener.listener->setPosition(currPos);
            listener.listener->setVelocity(currPos - prevPos);
        });

        //Source
        world.forEach([](TransformComponent const &transform, AudioSourceComponent &source) {
            vec3f const prevPos{ source.source->getPosition() };
            vec3f const currPos{ transform.getPosition() };

            source.source->setPosition(currPos);
            source.source->setVelocity(currPos - prevPos);
        });
    }
}