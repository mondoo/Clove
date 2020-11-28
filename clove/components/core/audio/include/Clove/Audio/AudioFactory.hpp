#pragma once

#include "Clove/Audio/AudioBuffer.hpp"
#include "Clove/Audio/AudioListener.hpp"
#include "Clove/Audio/AudioSource.hpp"

#include <Clove/Expected.hpp>

namespace garlic::clove {
    /**
	 * @brief Responsible for creating all audio objects.
	 */
    class AudioFactory {
        //FUNCTIONS
    public:
        virtual ~AudioFactory() = default;

        virtual Expected<std::unique_ptr<AudioBuffer>, std::exception> createAudioBuffer(AudioBuffer::Descriptor descriptor) = 0;
        virtual Expected<std::unique_ptr<AudioSource>, std::exception> createAudioSource()                                   = 0;
        virtual Expected<std::unique_ptr<AudioListener>, std::exception> createAudioListener()                               = 0;
    };
}
