#include "Clove/Audio/Audio.hpp"

#include "Clove/Audio/OpenAL/ALDevice.hpp"

namespace garlic::clove {
    std::unique_ptr<AudioDevice> createAudioDevice(AudioApi api) {
        switch(api) {
            case AudioApi::OpenAl:
                return std::make_unique<ALDevice>();
        }
    }
}