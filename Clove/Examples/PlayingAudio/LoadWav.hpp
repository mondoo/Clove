#pragma once

namespace WavLoader {
    char* loadWav(std::string_view fileName, uint8_t& channels, int32_t& sampleRate, uint8_t& bitsPerSample, size_t& size);
}