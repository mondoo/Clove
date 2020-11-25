#include "Clove/Audio/OpenAL/ALError.hpp"

#include <AL/alc.h>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    void printErrorAl(ALenum error, std::string_view const fileName, uint_fast32_t const line) {
        switch(error) {
            case AL_INVALID_NAME:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "AL_INVALID_NAME: A bad name (id) was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                break;
            case AL_INVALID_ENUM:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "AL_INVALID_ENUM: An invalid enum value was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                break;
            case AL_INVALID_VALUE:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "AL_INVALID_VALUE: An invalid value was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                break;
            case AL_INVALID_OPERATION:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "AL_INVALID_OPERATION: The requested operation was not valid. File: {0} Line {1}", fileName, line);
                break;
            case AL_OUT_OF_MEMORY:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "AL_OUT_OF_MEMORY: The requested operation caused OpenAL to run out of memory. File: {0} Line {1}", fileName, line);
                break;
        }
    }

    void printErrorAlc(ALenum error, std::string_view const fileName,  uint_fast32_t const line) {
        switch(error) {
            case ALC_INVALID_VALUE:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "ALC_INVALID_VALUE: An invalid value was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                break;
            case ALC_INVALID_DEVICE:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "ALC_INVALID_DEVICE: A bad device was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                break;
            case ALC_INVALID_CONTEXT:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "ALC_INVALID_CONTEXT: A bad context was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                break;
            case ALC_INVALID_ENUM:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "ALC_INVALID_ENUM: An unkown enum was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                break;
            case ALC_OUT_OF_MEMORY:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "ALC_OUT_OF_MEMORY: The requested operation caused OpenAL to run out of memory. File: {0} Line {1}", fileName, line);
                break;
        }
    }
}