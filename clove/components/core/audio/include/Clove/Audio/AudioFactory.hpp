#pragma once

#include "Clove/Audio/AudioBuffer.hpp"
#include "Clove/Audio/AudioListener.hpp"
#include "Clove/Audio/AudioSource.hpp"

#include <Clove/Expected.hpp>
#include <stdexcept>

namespace garlic::clove {
    /**
	 * @brief Responsible for creating all audio objects.
	 */
    class AudioFactory {
        //FUNCTIONS
    public:
        virtual ~AudioFactory() = default;

        virtual Expected<std::unique_ptr<AudioBuffer>, std::runtime_error> createAudioBuffer(AudioBuffer::Descriptor descriptor) = 0;
        virtual Expected<std::unique_ptr<AudioSource>, std::runtime_error> createAudioSource()                                   = 0;
        virtual Expected<std::unique_ptr<AudioListener>, std::runtime_error> createAudioListener()                               = 0;
    };
}
