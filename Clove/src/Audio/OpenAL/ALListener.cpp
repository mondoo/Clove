#include "Clove/Audio/OpenAL/ALListener.hpp"

namespace clv{
    ALListener::ALListener() = default;

    ALListener::ALListener(ALListener&& other) noexcept = default;

    ALListener& ALListener::operator=(ALListener&& other) noexcept = default;

    ALListener::~ALListener() = default;

    void ALListener::setPosition(const clv::mth::vec3f& position) {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
    }

    void ALListener::setVelocity(const clv::mth::vec3f& velocity) {
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }
}