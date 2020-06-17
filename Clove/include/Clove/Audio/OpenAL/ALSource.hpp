#pragma once

#include "Clove/Audio/AudioSource.hpp"

#include <AL/al.h>

namespace clv {
    class ALSource : public AudioSource {
        //VARIABLES
    private:
        ALuint source;

        //FUNCTIONS
    public:
        ALSource();

        ALSource(const ALSource& other) = delete;
        ALSource(ALSource&& other) noexcept;

        ALSource& operator=(const ALSource& other) = delete;
        ALSource& operator=(ALSource&& other) noexcept;

        ~ALSource();

        void setBuffer(const AudioBuffer& buffer) override;

        void setPitch(float pitch) override;
        void setLooping(bool isLooping) override;

        void setPosition(const clv::mth::vec3f& position) override;
        void setVelocity(const clv::mth::vec3f& velocity) override;

        void play() override;
        void pause() override;
        void stop() override;
        void rewind() override;
    };
}