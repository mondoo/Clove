#pragma once

#include "Clove/Audio/AudioBuffer.hpp"

#include <AL/al.h>

namespace clv {
    class ALBuffer : public AudioBuffer{
        //VARIABLES
    private:
        ALuint buffer;

        //FUNCTIONS
    public:
        ALBuffer() = delete;
        ALBuffer(AudioBufferDescriptor descriptor, const void* data, size_t dataSize);

        ALBuffer(const ALBuffer& other) = delete;
        ALBuffer(ALBuffer&& other) noexcept;

        ALBuffer& operator=(const ALBuffer& other) = delete;
        ALBuffer& operator=(ALBuffer&& other) noexcept;

        ~ALBuffer();
    };
}