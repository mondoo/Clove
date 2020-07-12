#include "Bulb/ECS/Components/AudioSourceComponent.hpp"

//#include "Bulb/Audio/SoundLoader.hpp"

#include <Clove/Audio/AudioBuffer.hpp>
#include <Clove/Audio/AudioSource.hpp>
#include <Clove/Audio/AudioFactory.hpp>

namespace blb::ecs {
    AudioSourceComponent::AudioSourceComponent(clv::AudioFactory& factory, std::string_view filePath){
        //buffer = aud::SoundLoader::loadSound(factory, filePath);
        source = factory.createAudioSource();

        source->setBuffer(*buffer);
    }

    AudioSourceComponent::AudioSourceComponent(AudioSourceComponent&& other) noexcept = default;

    AudioSourceComponent& AudioSourceComponent::operator=(AudioSourceComponent&& other) noexcept = default;

    AudioSourceComponent::~AudioSourceComponent() = default;

	void AudioSourceComponent::play(PlaybackMode playback) {

	}

	void AudioSourceComponent::pause() {

	}

	void AudioSourceComponent::resume() {

	}

	void AudioSourceComponent::stop() {

	}

	bool AudioSourceComponent::isPlaying() {
		return playing;
	}
}