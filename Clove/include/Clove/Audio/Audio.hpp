#pragma once

namespace clv {
    enum class AudioAPI {
        OpenAl
    };

    class AudioFactory;
}

namespace clv {
    std::unique_ptr<AudioFactory> createAudioFactory(AudioAPI api);
}