#pragma once

#include "Clove/Audio/AhaFactory.hpp"

namespace garlic::clove {
    class ALFactory : public AhaFactory {
        //FUNCTIONS
    public:
        ALFactory();

        ALFactory(ALFactory const &other) = delete;
        ALFactory(ALFactory &&other) noexcept;

        ALFactory &operator=(ALFactory const &other) = delete;
        ALFactory &operator=(ALFactory &&other) noexcept;

        ~ALFactory();

        Expected<std::unique_ptr<AhaBuffer>, std::runtime_error> createAudioBuffer(AhaBuffer::Descriptor descriptor) override;
        Expected<std::unique_ptr<AhaSource>, std::runtime_error> createAudioSource() override;
        Expected<std::unique_ptr<AhaListener>, std::runtime_error> createAudioListener() override;
    };
}
