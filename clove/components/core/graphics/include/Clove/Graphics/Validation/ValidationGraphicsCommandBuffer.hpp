#pragma once

namespace clove {
    class ValidationGraphicsCommandbufferInterface {
        //FUNCTIONS
    public:
        //Reuse validation
        virtual void setAllowBufferReuse(bool canReuse) = 0;
        virtual void markAsUsed()                       = 0;
    };

    template<typename BaseCommandBufferType>
    class ValidationGraphicsCommandBuffer : public BaseCommandBufferType, public ValidationGraphicsCommandbufferInterface {
        //VARIABLES
    private:
        bool allowReuse{ false };  /**< Will be true if this can be reused (recorded to multiple times without beeing freed) */
        bool hasBeenUsed{ false }; /**< Will be true if this buffer has been used before being rerecorded. */

        //FUNCTIONS
    public:
        using BaseCommandBufferType::BaseCommandBufferType;

        void beginRecording(CommandBufferUsage usageFlag) override;

        void setAllowBufferReuse(bool canReuse) override;
        void markAsUsed() override;
    };
}

#include "ValidationGraphicsCommandBuffer.inl"