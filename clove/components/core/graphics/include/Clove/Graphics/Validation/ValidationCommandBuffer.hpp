#pragma once

#include "Clove/Graphics/GhaComputeCommandBuffer.hpp"
#include "Clove/Graphics/GhaGraphicsCommandBuffer.hpp"
#include "clove/Graphics/GhaTransferCommandBuffer.hpp"

namespace clove {
    class ValidationCommandBuffer {
        //VARIABLES
    private:
        CommandBufferUsage currentUsage{ CommandBufferUsage::Default };
        bool allowReuse{ false };  /**< Will be true if this can be reused (recorded to multiple times without beeing freed) */
        bool hasBeenUsed{ false }; /**< Will be true if this buffer has been used before being rerecorded. */

        //FUNCTIONS
    public:
        void setAllowBufferReuse(bool canReuse);
        void markAsUsed();
        bool bufferHasBeenUsed();

        CommandBufferUsage getCommandBufferUsage() const;

    protected:
        void validateBeginRecording();
        void resetUsedFlag();

        void setCommandBufferUsage(CommandBufferUsage usage);
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