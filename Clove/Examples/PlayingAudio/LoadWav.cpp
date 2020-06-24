#include "LoadWav.hpp"

#include <Clove/Log.hpp>

#include <fstream>

namespace WavLoader {
    auto wavLogContext = clv::Log::createContext(clv::Log::Level::Error, "WAVLOADER", "ExampleLog.txt");

    static int32_t convertToInt(char* buffer, size_t len) {
        int32_t a = 0;
        std::memcpy(&a, buffer, len);
        return a;
    }

    static bool readHeader(std::ifstream& file, uint8_t& channels, int32_t& sampleRate, uint8_t& bitsPerSample, size_t& size) {
        char buffer[4];

        if(!file.is_open()) {
            return false;
        }

        // the RIFF
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read RIFF");
            return false;
        }
        if(std::strncmp(buffer, "RIFF", 4) != 0) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "File is not a valid WAVE file (header doesn't begin with RIFF)");
            return false;
        }

        // the size of the file
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read size of file");
            return false;
        }

        // the WAVE
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read WAVE");
            return false;
        }
        if(std::strncmp(buffer, "WAVE", 4) != 0) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "File is not a valid WAVE file (header doesn't contain WAVE)");
            return false;
        }

        // "fmt/0"
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read fmt/0");
            return false;
        }

        // this is always 16, the size of the fmt data chunk
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read the 16");
            return false;
        }

        // PCM should be 1?
        if(!file.read(buffer, 2)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read PCM");
            return false;
        }

        // the number of channels
        if(!file.read(buffer, 2)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read number of channels");
            return false;
        }
        channels = convertToInt(buffer, 2);

        // sample rate
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read sample rate");
            return false;
        }
        sampleRate = convertToInt(buffer, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read (sampleRate * bitsPerSample * channels) / 8");
            return false;
        }

        // ?? dafaq
        if(!file.read(buffer, 2)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read dafaq");
            return false;
        }

        // bitsPerSample
        if(!file.read(buffer, 2)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read bits per sample");
            return false;
        }
        bitsPerSample = convertToInt(buffer, 2);

        // data chunk header "data"
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read data chunk header");
            return false;
        }
        if(std::strncmp(buffer, "data", 4) != 0) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "File is not a valid WAVE file (doesn't have 'data' tag)");
            return false;
        }

        // size of data
        if(!file.read(buffer, 4)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read data size");
            return false;
        }
        size = convertToInt(buffer, 4);

        /* cannot be at the end of file */
        if(file.eof()) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Reached EOF on the file");
            return false;
        }
        if(file.fail()) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Fail state set on the file");
            return false;
        }

        return true;
    }

    char* loadWav(std::string_view fileName, uint8_t& channels, int32_t& sampleRate, uint8_t& bitsPerSample, size_t& size) {
        std::ifstream file(fileName.data(), std::ios::binary);
        if(!file.is_open()) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not open file: {0}", fileName.data());
        }

        if(!readHeader(file, channels, sampleRate, bitsPerSample, size)) {
            GARLIC_LOG(wavLogContext, clv::Log::Level::Error, "Could not read wav header of file: {0}", fileName.data()); 
        }

        char* data = new char[size];
        file.read(data, size);

        return data;
    }
}