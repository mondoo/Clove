
namespace clove {
    //Graphics
    template<typename BaseCommandBufferType>
    void ValidationGraphicsCommandBuffer<BaseCommandBufferType>::beginRecording(CommandBufferUsage usageFlag) {
        validateBeginRecording();

        setCommandBufferUsage(usageFlag);
        resetUsedFlag();

        BaseCommandBufferType::beginRecording(usageFlag);
    }

    template<typename BaseCommandBufferType>
    void ValidationGraphicsCommandBuffer<BaseCommandBufferType>::endRecording() {
        onEndRecording();

        BaseCommandBufferType::endRecording();
    }

    //Compute
    template<typename BaseCommandBufferType>
    void ValidationComputeCommandBuffer<BaseCommandBufferType>::beginRecording(CommandBufferUsage usageFlag) {
        validateBeginRecording();

        setCommandBufferUsage(usageFlag);
        resetUsedFlag();

        BaseCommandBufferType::beginRecording(usageFlag);
    }

    template<typename BaseCommandBufferType>
    void ValidationComputeCommandBuffer<BaseCommandBufferType>::endRecording() {
        onEndRecording();

        BaseCommandBufferType::endRecording();
    }

    //Transfer
    template<typename BaseCommandBufferType>
    void ValidationTransferCommandBuffer<BaseCommandBufferType>::beginRecording(CommandBufferUsage usageFlag) {
        validateBeginRecording();

        setCommandBufferUsage(usageFlag);
        resetUsedFlag();

        BaseCommandBufferType::beginRecording(usageFlag);
    }

    template<typename BaseCommandBufferType>
    void ValidationTransferCommandBuffer<BaseCommandBufferType>::endRecording() {
        onEndRecording();

        BaseCommandBufferType::endRecording();
    }
}