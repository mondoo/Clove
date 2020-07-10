#pragma once

#include "Clove/Audio/AudioBuffer.hpp"

#include <AL/al.h>

namespace clv {
    class ALBuffer : public AudioBuffer{
        //VARIABLES
    private:
        ALuint buffer = 0;

        //FUNCTIONS
    public:
        ALBuffer();
        ALBuffer(const AudioDataDescriptor& descriptor);

        ALBuffer(const ALBuffer& other) = delete;
        ALBuffer(ALBuffer&& other) noexcept;

        ALBuffer& operator=(const ALBuffer& other) = delete;
        ALBuffer& operator=(ALBuffer&& other) noexcept;

        ~ALBuffer();

        void map(const AudioDataDescriptor& descriptor) override;

        ALuint getBufferId() const;
    };
}