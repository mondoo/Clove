#pragma once

namespace clv {
    class AudioListener {
        //FUNCTIONS
    public:
        virtual ~AudioListener() = default;

        virtual void setPosition(const clv::mth::vec3f& position) = 0;
        virtual void setVelocity(const clv::mth::vec3f& velocity) = 0;
    };
}