#include "Clove/Audio/OpenAL/ALListener.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"

namespace garlic::clove {
    ALListener::ALListener() = default;

    ALListener::ALListener(ALListener &&other) noexcept = default;

    ALListener &ALListener::operator=(ALListener &&other) noexcept = default;

    ALListener::~ALListener() = default;

    void ALListener::setPosition(vec3f const &position) {
        alCall(alListener3f(AL_POSITION, position.x, position.y, position.z));
    }

    void ALListener::setVelocity(vec3f const &velocity) {
        alCall(alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z));
    }

    vec3f ALListener::getPosition() const {
        vec3f position{};
        alCall(alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z));
        return position;
    }

    vec3f ALListener::getVelocity() const {
        vec3f velocity{};
        alCall(alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z));
        return velocity;
    }
}