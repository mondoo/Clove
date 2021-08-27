#pragma once

namespace clove {
    template<typename BaseBufferType>
    class ValidationBuffer : public BaseBufferType {
        //FUNCTIONS
    public:
        using BaseBufferType::BaseBufferType;

        void write(void const *data, size_t const offset, size_t const size) override;

        void read(void *data, size_t const offset, size_t const size) override;
    };
}

#include "ValidationBuffer.inl"