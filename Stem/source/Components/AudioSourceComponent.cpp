#include "Stem/Components/AudioSourceComponent.hpp"

#include <Clove/Audio/AudioBuffer.hpp>
#include <Clove/Audio/AudioFactory.hpp>
#include <Clove/Audio/AudioSource.hpp>

namespace garlic::inline stem {
    AudioSourceComponent::AudioSourceComponent(clv::AudioFactory &factory) {
        source = factory.createAudioSource();
    }

    AudioSourceComponent::AudioSourceComponent(AudioSourceComponent &&other) noexcept = default;

    AudioSourceComponent &AudioSourceComponent::operator=(AudioSourceComponent &&other) noexcept = default;

    AudioSourceComponent::~AudioSourceComponent() = default;

    void AudioSourceComponent::setBuffer(std::shared_ptr<clv::AudioBuffer> buffer) {
        source->setBuffer(std::move(buffer));
    }

    void AudioSourceComponent::queueBuffers(std::vector<std::shared_ptr<clv::AudioBuffer>> buffers) {
        source->queueBuffers(std::move(buffers));
    }

    std::vector<std::shared_ptr<clv::AudioBuffer>> AudioSourceComponent::unQueueBuffers(uint32_t const numToUnQueue) {
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