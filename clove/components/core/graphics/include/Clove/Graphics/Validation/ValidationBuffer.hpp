#pragma once

#include "Clove/Graphics/GhaBuffer.hpp"

#include <memory>

namespace clove {
    class ValidationBuffer : public GhaBuffer {
        //VARIABLES
    private:
        Descriptor descriptor{};
        std::unique_ptr<GhaBuffer> internalBuffer{};

        //FUNCTIONS
    public:
        ValidationBuffer() = delete;
        ValidationBuffer(Descriptor descriptor, std::unique_ptr<GhaBuffer> buffer);

        ValidationBuffer(ValidationBuffer const &other) = delete;
        ValidationBuffer(ValidationBuffer &&other) noexcept;

        ValidationBuffer &operator=(ValidationBuffer const &other) = delete;
        ValidationBuffer &operator=(ValidationBuffer &&other) noexcept;

        ~ValidationBuffer();

        void write(void const *data, size_t const offset, size_t const size) override;

        void read(void *data, size_t const offset, size_t const size) override;
    };
}