#pragma once

#include <memory>

namespace garlic::clove {
    enum class AudioAPI {
        OpenAl
    };

    class AudioFactory;
}

namespace garlic::clove {
    std::unique_ptr<AudioFactory> createAudioFactory(AudioAPI api);
}