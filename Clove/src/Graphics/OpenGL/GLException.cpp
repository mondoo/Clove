#include "Clove/Graphics/OpenGL/GLException.hpp"

#include <Root/Log/Log.hpp>
#include <glad/glad.h>

namespace clv::gfx::ogl {
    void GARLIC_APIENTRY errorCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* msg, const void* data) {
        switch(severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, msg);
                break;

            case GL_DEBUG_SEVERITY_MEDIUM:
            case GL_DEBUG_SEVERITY_LOW:
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, msg);
                break;
        }
    }
}