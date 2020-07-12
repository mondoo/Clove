#pragma once

namespace clv {
	/**
	 * @brief Handle to the listener in the 'audio scene'
	 * @details Only one listener is active when playing audio. This class
	 * exposes functions for setting values on the listener object. Such as
	 * setting the position
	 */
    class AudioListener {
        //FUNCTIONS
    public:
        virtual ~AudioListener() = default;

        virtual void setPosition(const clv::mth::vec3f& position) = 0;
        virtual void setVelocity(const clv::mth::vec3f& velocity) = 0;

        virtual clv::mth::vec3f getPosition() const = 0;
        virtual clv::mth::vec3f getVelocity() const = 0;
    };
}
