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
        ALBuffer(const DataInfo& info);

        ALBuffer(const ALBuffer& other) = delete;
        ALBuffer(ALBuffer&& other) noexcept;

        ALBuffer& operator=(const ALBuffer& other) = delete;
        ALBuffer& operator=(ALBuffer&& other) noexcept;

        ~ALBuffer();

        void map(const DataInfo& info) override;

        ALuint getBufferId() const;
    };
}
