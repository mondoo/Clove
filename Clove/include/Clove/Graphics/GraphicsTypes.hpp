#pragma once

#include <Root/Definitions.hpp>

namespace clv::gfx {
    enum class API {
        None,
#if GARLIC_PLATFORM_WINDOWS
        Vulkan,
#elif GARLIC_PLATFORM_LINUX
        Vulkan,
#elif GARLIC_PLATFORM_MACOS
    //TODO: Metal
#endif
    };

    enum class ShaderStage {
        Vertex,
        Pixel,
        Geometry
    };

    enum class VertexElementType {
        position2D,
        position3D,
        texture2D,
        normal,
        colour3D,
        jointIds,
        weights,
    };

    struct Viewport {
        int32_t x{ 0 };
        int32_t y{ 0 };
        int32_t width{ 0 };
        int32_t height{ 0 };
    };

    //TODO: Have Success and Error then OR those with other codes so you can just check for succes or error
    enum class Result {
        Success,
        Success_SwapchainSuboptimal,

        Error_SwapchainOutOfDate,

        Unkown
    };

    enum class ImageFormat {
        Unkown,

        R8G8B8A8_SRGB,
        B8G8R8A8_SRGB,
        B8G8R8A8_UNORM,

        D32_SFLOAT
    };

    enum class ImageLayout {
        Undefined,
        General,
        Present,
        TransferDestinationOptimal,
        ShaderReadOnlyOptimal,
        ColourAttachmentOptimal,
        DepthStencilAttachmentOptimal,
        DepthStencilReadOnlyOptimal
    };

    enum class VertexAttributeFormat {
        R32_SFLOAT,
        R32G32_SFLOAT,
        R32G32B32_SFLOAT,
        R32G32B32A32_SFLOAT
    };

    struct VertexInputBindingDescriptor {
        uint32_t binding{ 0 }; /**< Which binding in the shader this input describes */
        uint32_t stride{ 0 };
    };

    struct VertexAttributeDescriptor {
        uint32_t binding{ 0 }; /**< Binding that the data is taken from when bound with a command buffer */
        uint32_t location{ 0 };
        VertexAttributeFormat format;
        uint32_t offset{ 0 };
    };

    enum class DescriptorType {
        UniformBuffer,
        CombinedImageSampler,
    };

    enum class SharingMode {
        Exclusive, /**< Can only be accessed by a single queue type at a time. The ownership must be explicitly transfered between queues. */
        Concurrent /**< Can be accessed between different queue types without transferring ownership. */
    };

    enum class MemoryType {
        VideoMemory,  /**< Can't be written to be CPU, GPU optimised */
        SystemMemory, /**< Can be written to by CPU, not GPU optimised */
    };

    enum class PipelineStage {
        Top,
        Transfer,
        PixelShader,
        EarlyPixelTest,
        ColourAttachmentOutput,
    };

    using AccessFlagsType = uint16_t;
    /**
     * @brief Memory access types that will participate in a memory dependency.
     */
    enum class AccessFlags : AccessFlagsType {
        None                        = 0,
        TransferWrite               = 1 << 0,
        ShaderRead                  = 1 << 1,
        ColourAttachmentWrite       = 1 << 2,
        DepthStencilAttachmentRead  = 1 << 3,
        DepthStencilAttachmentWrite = 1 << 4
    };
    GARLIC_ENUM_BIT_FLAG_OPERATORS(AccessFlags, AccessFlagsType)

    enum class LoadOperation {
        DontCare,
        Clear,
    };

    enum class StoreOperation {
        DontCare,
        Store,
    };

    struct RenderArea {
        mth::vec2i origin;
        mth::vec2ui size;
    };

    struct DepthStencilValue {
        float depth{ 0.0f };
        uint32_t stencil{ 0 };
    };

    enum class CommandBufferUsage {
        Default,
        OneTimeSubmit,
    };

    enum class IndexType {
        Uint16
    };

    enum class QueueFlags {
        None,
        Transient, /**< Buffers will be short lived */
        ReuseBuffers /**< Buffers can be reused */
    };

    struct CommandQueueDescriptor {
        QueueFlags flags;
    };

    enum class WaitStage {
        ColourAttachmentOutput
    };

    enum class QueueType {
        None,
        Graphics,
        Transfer
    };

    struct BufferMemoryBarrierInfo {
        AccessFlags sourceAccess;
        AccessFlags destinationAccess;
        QueueType sourceQueue{ QueueType::None };      /**< If set signifies which queue this is being transfered from. */
        QueueType destinationQueue{ QueueType::None }; /**< If set signifies which queue this is being transfered to. */ 
    };

    struct ImageMemoryBarrierInfo {
        AccessFlags sourceAccess;
        AccessFlags destinationAccess;
        ImageLayout oldImageLayout;
        ImageLayout newImageLayout;
        QueueType sourceQueue{ QueueType::None };      /**< If set signifies which queue this is being transfered from. */
        QueueType destinationQueue{ QueueType::None }; /**< If set signifies which queue this is being transfered to. */ 
    };
}
