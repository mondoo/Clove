#pragma once

#include <Bulb/ECS/Components/AudioListenerComponent.hpp>

#include "Bulb/ECS/System.hpp"

namespace blb::ecs {
	class AudioSystem : public System {
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		AudioSystem();

		AudioSystem(const AudioSystem& other) = delete;
		AudioSystem(AudioSystem&& other) noexcept;

		AudioSystem& operator=(const AudioSystem& other) = delete;
		AudioSystem& operator=(AudioSystem&& other) noexcept;

		~AudioSystem();

		void update(World& world, clv::utl::DeltaTime deltaTime) override;
	};
}