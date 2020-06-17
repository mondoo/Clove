#pragma once

namespace clv {
    class AudioBuffer;
}

namespace clv {
    class AudioSource {
        //FUNCTIONS
    public:
        virtual ~AudioSource() = default;

        virtual void setBuffer(const AudioBuffer& buffer) = 0;

        virtual void setPitch(float pitch)                = 0;
        virtual void setLooping(bool isLooping)           = 0;

        virtual void setPosition(const clv::mth::vec3f& position) = 0;
        virtual void setVelocity(const clv::mth::vec3f& velocity) = 0;

        virtual void play()   = 0;
        virtual void pause()  = 0;
        virtual void stop()   = 0;
        virtual void rewind() = 0;
    };
}