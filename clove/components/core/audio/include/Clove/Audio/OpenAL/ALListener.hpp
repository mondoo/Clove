#pragma once

#include "Clove/Audio/AudioListener.hpp"

#include <AL/al.h>

namespace garlic::clove {
    class ALListener : public AudioListener {
        //FUNCTIONS
    public:
        ALListener();

        ALListener(ALListener const &other) = delete;
        ALListener(ALListener &&other) noexcept;

        ALListener &operator=(ALListener const &other) = delete;
        ALListener &operator=(ALListener &&other) noexcept;

        ~ALListener();

        void setPosition(vec3f const &position) override;
        void setVelocity(vec3f const &velocity) override;

        vec3f getPosition() const override;
        vec3f getVelocity() const override;
    };
}