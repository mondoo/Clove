#include "Bulb/ECS/Systems/AudioSystem.hpp"

#include "Bulb/ECS/Components/AudioSourceComponent.hpp"
#include "Bulb/ECS/Components/AudioListenerComponent.hpp"
#include "Bulb/ECS/Components/TransformComponent.hpp"
#include "Bulb/ECS/World.hpp"

using namespace clv;

namespace blb::ecs {
	AudioSystem::AudioSystem() = default;

	AudioSystem::AudioSystem(AudioSystem&& other) noexcept = default;

	AudioSystem& AudioSystem::operator=(AudioSystem&& other) noexcept = default;

	AudioSystem::~AudioSystem() = default;

	void AudioSystem::update(World& world, utl::DeltaTime deltaTime) {
		CLV_PROFILE_FUNCTION();

		for(auto&& [transform, listener] : world.getComponentSets<TransformComponent, AudioListenerComponent>()){
		    //TODO:
		}

		for(auto&& [transform, audio] : world.getComponentSets<TransformComponent, AudioSourceComponent>()) {
            //TODO
		}
	}
}