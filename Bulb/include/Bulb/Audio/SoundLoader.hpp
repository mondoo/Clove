#pragma once

namespace clv {
    class AudioFactory;
    class AudioBuffer;
}

namespace blb::aud::SoundLoader {
    std::unique_ptr<clv::AudioBuffer> loadSound(clv::AudioFactory& factory, std::string_view filePath);
}