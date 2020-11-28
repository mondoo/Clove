#pragma once

#include <memory>

namespace garlic::clove {
    enum class AudioApi {
        OpenAl
    };

    class AudioDevice;
}

namespace garlic::clove {
    std::unique_ptr<AudioDevice> createAudioDevice(AudioApi api);
}