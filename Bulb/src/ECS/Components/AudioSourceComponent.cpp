#include "Bulb/ECS/Components/AudioSourceComponent.hpp"

#include <Clove/Audio/AudioBuffer.hpp>
#include <Clove/Audio/AudioSource.hpp>
#include <Clove/Audio/AudioFactory.hpp>

namespace blb::ecs {
    AudioSourceComponent::AudioSourceComponent(clv::AudioFactory& factory){
        source = factory.createAudioSource();
    }

    AudioSourceComponent::AudioSourceComponent(AudioSourceComponent&& other) noexcept = default;

    AudioSourceComponent& AudioSourceComponent::operator=(AudioSourceComponent&& other) noexcept = default;

    AudioSourceComponent::~AudioSourceComponent() = default;

    void AudioSourceComponent::setBuffer(const clv::AudioBuffer& buffer) {
        source->setBuffer(buffer);
    }

    void AudioSourceComponent::queueBuffers(std::vector<std::shared_ptr<clv::AudioBuffer>> buffers) {
        source->queueBuffers(std::move(buffers));
    }

    std::vector<std::shared_ptr<clv::AudioBuffer>> AudioSourceComponent::unQueueBuffers(const uint32_t numToUnQueue) {
        return source->unQueueBuffers(numToUnQueue);
    }

	void AudioSourceComponent::play(PlaybackMode playback) {
        source->setLooping(playback == PlaybackMode::Repeat);
        source->play();
	}

	void AudioSourceComponent::pause() {
        source->pause();
	}

	void AudioSourceComponent::resume() {
        source->play();
	}

	void AudioSourceComponent::stop() {
        source->stop();
	}
}