#pragma once

#include "Clove/Audio/AudioBuffer.hpp"

#include <AL/al.h>

namespace garlic::clove {
    class ALBuffer : public AudioBuffer {
        //VARIABLES
    private:
        ALuint buffer{ 0 };
        Descriptor descriptor;

        //FUNCTIONS
    public:
        ALBuffer() = delete;
        ALBuffer(ALuint buffer, Descriptor descriptor);

        ALBuffer(ALBuffer const &other) = delete;
        ALBuffer(ALBuffer &&other) noexcept;

        ALBuffer &operator=(ALBuffer const &other) = delete;
        ALBuffer &operator=(ALBuffer &&other) noexcept;

        ~ALBuffer();

        void write(void const *data, size_t const size) override;

        ALuint getBufferId() const;
    };
}
