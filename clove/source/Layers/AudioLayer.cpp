#include "Clove/Layers/AudioLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Components/AudioListenerComponent.hpp"
#include "Clove/Components/AudioSourceComponent.hpp"
#include "Clove/Components/TransformComponent.hpp"

#include <Clove/Audio/AudioListener.hpp>
#include <Clove/Audio/AudioSource.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    AudioLayer::AudioLayer()
        : Layer("Audio")
        , entityManager{ Application::get().getEntityManager()} {

    }

    AudioLayer::AudioLayer(AudioLayer &&other) noexcept = default;

    AudioLayer &AudioLayer::operator=(AudioLayer &&other) noexcept = default;

    AudioLayer::~AudioLayer() = default;

    void AudioLayer::onUpdate(DeltaTime const deltaTime) {
        CLOVE_PROFILE_FUNCTION();

        //Listener
        entityManager->forEach([](TransformComponent const &transform, AudioListenerComponent &listener) {
            vec3f const prevPos{ listener.listener->getPosition() };
            vec3f const currPos{ transform.getPosition() };

            listener.listener->setPosition(currPos);
            listener.listener->setVelocity(currPos - prevPos);
        });

        //Source
        entityManager->forEach([](TransformComponent const &transform, AudioSourceComponent &source) {
            vec3f const prevPos{ source.source->getPosition() };
            vec3f const currPos{ transform.getPosition() };

            source.source->setPosition(currPos);
            source.source->setVelocity(currPos - prevPos);
        });
    }
}