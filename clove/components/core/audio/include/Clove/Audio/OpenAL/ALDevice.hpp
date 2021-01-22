#pragma once

#include "Clove/Audio/AhaDevice.hpp"

#include <AL/alc.h>

namespace garlic::clove {
    class ALFactory;
}

namespace garlic::clove {
    /**
     * @brief Represents the actual hardware used for audio rendering. Can be quieried for info about the device.
     */
    class ALDevice : public AudioDevice {
        //VARIABLES
    private:
        ALCdevice *alDevice{ nullptr };
        ALCcontext *alContext{ nullptr };

        std::shared_ptr<ALFactory> factory;

        //FUNCTIONS
    public:
        ALDevice();

        ALDevice(ALDevice const &other) = delete;
        ALDevice(ALDevice &&other) noexcept;

        ALDevice &operator=(ALDevice const &other) = delete;
        ALDevice &operator=(ALDevice &&other) noexcept;

        ~ALDevice();

        std::shared_ptr<AhaFactory> getAudioFactory() const override;
    };
}