#pragma once

#include "Clove/Graphics/GhaGraphicsCommandBuffer.hpp"
#include "Clove/Graphics/GhaComputeCommandBuffer.hpp"
#include "clove/Graphics/GhaTransferCommandBuffer.hpp"

namespace clove {
    class ValidationCommandBuffer {
        //VARIABLES
    private:
        //Reuse validation
        bool allowReuse{ false };  /**< Will be true if this can be reused (recorded to multiple times without beeing freed) */
        bool hasBeenUsed{ false }; /**< Will be true if this buffer has been used before being rerecorded. */

        //FUNCTIONS
    public:
        //Reuse validation
        void setAllowBufferReuse(bool canReuse);
        void markAsUsed();

    protected:
        void validateBeginRecording();
    };

    template<typename BaseCommandBufferType>
    class ValidationGraphicsCommandBuffer : public BaseCommandBufferType, public ValidationCommandBuffer {
        //FUNCTIONS
    public:
        using BaseCommandBufferType::BaseCommandBufferType;

        void beginRecording(CommandBufferUsage usageFlag) override;
    };

    template<typename BaseCommandBufferType>
    class ValidationComputeCommandBuffer : public BaseCommandBufferType, public ValidationCommandBuffer {
        //FUNCTIONS
    public:
        using BaseCommandBufferType::BaseCommandBufferType;

        void beginRecording(CommandBufferUsage usageFlag) override;
    };

    template<typename BaseCommandBufferType>
    class ValidationTransferCommandBuffer : public BaseCommandBufferType, public ValidationCommandBuffer {
        //FUNCTIONS
    public:
        using BaseCommandBufferType::BaseCommandBufferType;

        void beginRecording(CommandBufferUsage usageFlag) override;
    };
}

#include "ValidationCommandBuffer.inl"