#include "Clove/Audio/OpenAL/ALListener.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"

#include <Clove/Maths/Vector.hpp>

namespace clv {
    ALListener::ALListener() = default;

    ALListener::ALListener(ALListener&& other) noexcept = default;

    ALListener& ALListener::operator=(ALListener&& other) noexcept = default;

    ALListener::~ALListener() = default;

    void ALListener::setPosition(const clv::mth::vec3f& position) {
        alCall(alListener3f(AL_POSITION, position.x, position.y, position.z));
    }

    void ALListener::setVelocity(const clv::mth::vec3f& velocity) {
        alCall(alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z));
    }

    mth::vec3f ALListener::getPosition() const {
        mth::vec3f position{};
        alCall(alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z));
        return position;
    }

    mth::vec3f ALListener::getVelocity() const {
        mth::vec3f velocity{};
        alCall(alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z));
        return velocity;
    }
}