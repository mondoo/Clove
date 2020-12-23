#include "Membrane/Log.hpp"

#include <Clove/Log/Log.hpp>
#include <msclr\marshal_cppstd.h>

CLOVE_DECLARE_LOG_CATEGORY(BULB);

namespace garlic::membrane{
    namespace {
        clove::LogLevel convertLevel(LogLevel level){
            switch (level)
            {
                default:
                case LogLevel::Trace:
                    return clove::LogLevel::Trace;
                case LogLevel::Info:
                    return clove::LogLevel::Info;
                case LogLevel::Warning:
                    return clove::LogLevel::Warning;
                case LogLevel::Error:
                    return clove::LogLevel::Error;
            }
        }
    }
    void Log::write(LogLevel level, System::String ^ message){
        CLOVE_LOG(LOG_CATEGORY_BULB, convertLevel(level), msclr::interop::marshal_as<std::string>(message));
    }
}