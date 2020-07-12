#pragma once

#include <AL/al.h>

namespace clv {
    void printErrorAl(ALenum error, const std::string_view fileName, const uint_fast32_t line);
    void printErrorAlc(ALenum error, const std::string_view fileName, const uint_fast32_t line);
}

#define alCall(call)                                 \
    call;                                            \
    {                                                \
        ALenum error = alGetError();                 \
        if(error != AL_NO_ERROR) {                   \
            printErrorAl(error, __FILE__, __LINE__); \
        }                                            \
    }

#define alcCall(call, device)                         \
    call;                                             \
    {                                                 \
        ALenum error = alcGetError(device);           \
        if(error != AL_NO_ERROR) {                    \
            printErrorAlc(error, __FILE__, __LINE__); \
        }                                             \
    }
