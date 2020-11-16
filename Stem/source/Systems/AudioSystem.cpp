#include "Stem/Systems/AudioSystem.hpp"

#include "Stem/Components/AudioListenerComponent.hpp"
#include "Stem/Components/AudioSourceComponent.hpp"
#include "Stem/Components/TransformComponent.hpp"

#include <Bulb/ECS/World.hpp>
#include <Clove/Audio/AudioListener.hpp>
#include <Clove/Audio/AudioSource.hpp>
#include <Root/Log/Log.hpp>

using namespace clv;

namespace garlic::inline stem {
    AudioSystem::AudioSystem() = default;

    AudioSystem::AudioSystem(AudioSystem &&other) noexcept = default;

    AudioSystem &AudioSystem::operator=(AudioSystem &&other) noexcept = default;

    AudioSystem::~AudioSystem() = default;

    void AudioSystem::update(blb::ecs::World &world, utl::DeltaTime deltaTime) {
        CLV_PROFILE_FUNCTION();

        //Listener
        auto const listenerSet{ world.getComponentSets<TransformComponent, AudioListenerComponent>() };
        size_t const numListeners{ std::size(listenerSet) };
        GARLIC_ASSERT(numListeners <= 1, "Only one listener is allowed per world");
        if(numListeners > 0) {
            auto &&[transform, listener] = listenerSet[0];

            mth::vec3f const prevPos{ listener->listener->getPosition() };
            mth::vec3f const currPos{ transform->getPosition() };

            listener->listener->setPosition(currPos);
            listener->listener->setVelocity(currPos - prevPos);
        }

        //Source
        for(auto &&[transform, source] : world.getComponentSets<TransformComponent, AudioSourceComponent>()) {
            mth::vec3f const prevPos{ source->source->getPosition() };
            mth::vec3f const currPos{ transform->getPosition() };

            source->source->setPosition(currPos);
            source->source->setVelocity(currPos - prevPos);
        }
    }
}