#include "Clove/Audio/Audio.hpp"

#include "Clove/Audio/OpenAL/ALFactory.hpp"

namespace garlic::clove {
    std::unique_ptr<AudioFactory> createAudioFactory(AudioAPI api) {
        switch(api) {
            case AudioAPI::OpenAl:
                return std::make_unique<ALFactory>();
        }
    }
}