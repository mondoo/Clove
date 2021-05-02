#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgId.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"
#include "Clove/Rendering/RenderGraph/RgShader.hpp"

#include <Clove/Graphics/GhaBuffer.hpp>
#include <Clove/Graphics/GhaGraphicsPipelineObject.hpp>
#include <Clove/Graphics/GhaGraphicsQueue.hpp>
#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>
#include <Clove/Graphics/GhaFramebuffer.hpp>
#include <Clove/Maths/Vector.hpp>
#include <filesystem>
#include <functional>
#include <unordered_map>
#include <vector>

namespace garlic::clove {
    class RgFrameCache;
    class RgGlobalCache;

    class GhaFactory;
    class GhaComputeQueue;
    class GhaGraphicsQueue;
    class GhaTransferQueue;
    class GhaGraphicsCommandBuffer;
    class GhaComputeCommandBuffer;
    class GhaTransferCommandBuffer;
}

namespace garlic::clove {
    /**
     * @brief Tracks how resources are used to create a graph of how to render a frame.
     */
    class RenderGraph {
        //TYPES
    public:
        struct BufferBinding {
            uint32_t slot{};
            RgBuffer buffer{};
        };

        struct ImageBindng {
            uint32_t slot{};
            GhaSampler::Descriptor samplerState{};//TODO: RG version?
            RgImage image{};
        };

        struct RenderTargetBinding {//TODO: RenderTargetDescriptor?
            LoadOperation loadOp{};
            StoreOperation storeOp{};
            RgImage target{};
        };

        struct DepthStencilBinding {
            LoadOperation loadOp{};
            StoreOperation storeOp{};
            RgImage target{};
        };

        struct GraphicsPassDescriptor {
            RgShader vertexShader;
            RgShader pixelShader;

            //NOTE: Will create the vertex input based off of clove's uber vertex

            //TODO: Viewport / scissor
            //TODO: raster state
            //TODO: depth state
            //TODO: Blending

            //NOTE: Will be used to create the render pass.
            std::vector<RenderTargetBinding> renderTargets{};
            DepthStencilBinding depthStencil{};
        };
        struct ComputePassDescriptor {
            //TODO
        };

        /**
         * @brief Represents a unit of work, single draw indexed call.
         */
        struct GraphicsSubmission {
            RgBuffer vertexBuffer{};
            RgBuffer indexBuffer{};

            //NOTE: Will be used to create descriptor sets
            std::vector<BufferBinding> shaderUbos{};
            std::vector<ImageBindng> shaderCombinedImageSamplers{};

            size_t indexCount{ 0 };
        };

        /**
         * @brief Represents a single dispatch call.
         */
        struct ComputeSubmission {
            vec3ui dispatchSize{ 1, 1, 1 };

            //NOTE: Will be used to create descriptor sets
            std::vector<BufferBinding> shaderStorageBuffers{};
            std::vector<ImageBindng> shaderStorageImages{};//TODO: Sampler not required on compute passes
        };

    private:
        struct BufferWrite {
            std::vector<std::byte> data;
            size_t offset{};
            size_t size;
        };

        //VARIABLES
    private:
        RgFrameCache &frameCache;
        RgGlobalCache &globalCache;
        ResourceIdType nextId{ 1 };

        std::vector<std::function<void(GhaGraphicsCommandBuffer &, GhaComputeCommandBuffer &, GhaTransferCommandBuffer&)>> operations{}; /**< Every operation (pass, copy, write, etc.) recorded into the graph in order. */

        std::unordered_map<ResourceIdType, GhaBuffer::Descriptor> bufferDescriptors{};
        std::unordered_map<ResourceIdType, BufferWrite> bufferWrites{};
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaBuffer>> allocatedBuffers{}; /**< All active buffers. Even external ones. */

        std::unordered_map<ResourceIdType, GhaImage::Descriptor> imageDescriptors{};
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaImage>> allocatedImages{};         /**< All active images. Even external ones. */
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaImageView>> allocatedImageViews{}; /**< All active images. Even external ones. */

        std::unordered_map<ResourceIdType, GraphicsPassDescriptor> passDescriptors{};
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaShader>> allocatedShaders{};
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaRenderPass>> allocatedRenderPasses{};
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaFramebuffer>> allocatedFramebuffers{};
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaGraphicsPipelineObject>> allocatedPipelines{};
        std::unordered_map<ResourceIdType, std::shared_ptr<GhaDescriptorSet>> allocatedDescriptorSets{};

        //FUNCTIONS
    public:
        RenderGraph() = delete;
        RenderGraph(RgFrameCache &frameCache, RgGlobalCache &globalCache);

        RenderGraph(RenderGraph const &other);
        RenderGraph(RenderGraph &&other) noexcept;

        RenderGraph &operator=(RenderGraph const &other);
        RenderGraph &operator=(RenderGraph &&other) noexcept;

        ~RenderGraph();

        /**
         * @brief Constructs an RgBuffer with the specified size.
         * @param bufferSize 
         * @return 
         */
        RgBuffer createBuffer(size_t bufferSize);
        /**
         * @brief Constructs an RgBuffer from an existing GhaBuffer. Usefull if wanting to use
         * pre set up buffers (such as vertex / index buffers).
         * @param buffer GhaBuffer to construct from.
         * @param offset The offset into the GhaBuffer that this RgBuffer will view.
         * @param size The size of the GhaBuffer that this RgBuffer will view.
         * @return
         */
        RgBuffer createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t offset, size_t size);

        /**
         * @brief Write data into this buffer.
         * @param data Pointer to data to write.
         * @param offset Offset into the buffer to write to. If the RgBuffer views an offset into a GhaBuffer then it'll be view offset + offset
         * @param size Size of the region in the buffer to write to.
         */
        void writeToBuffer(RgBuffer const &buffer, void const *data, size_t const offset, size_t const size);

        /**
         * @brief Constructs a new RgImage with the specified type and dimensions.
         * @param imageType 
         * @param format
         * @param dimensions 
         * @return 
         */
        RgImage createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions);
        /**
         * @brief Creates a new RgImage from an existing image view. Usefull
         * if wanting to use pre made images (such as backbuffers) in the render graph.
         * @param ghaImageView 
         * @return 
         */
        RgImage createImage(std::shared_ptr<GhaImageView> ghaImageView);

        /**
         * @brief Creates a new RgShader from a source file.
         * @param file 
         * @param shaderStage 
         * @return 
         */
        RgShader createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage);
        /**
         * @brief Create a new RgShader from a source string.
         * @param source 
         * @param includeSources A map of source strings that will get searched for any #includes.
         * @param shaderName A name for the shader being compiled.
         * @param shaderStage 
         * @return 
         */
        RgShader createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage);

        void addGraphicsPass(GraphicsPassDescriptor passDescriptor, std::vector<GraphicsSubmission> pass);
        void addComputePass(ComputePassDescriptor const &passDescriptor, std::vector<ComputeSubmission> pass);

        /**
         * @brief Executes the RenderGraph. Creating any objects required and submitting commands to the relevant queues.
         * @param factory Requires a factory to create any GHA objects when traversing the graph.
         * @param graphicsQueue The queue the graph will submit graphics work to.
         * @param computeQueue The queue the graph will submit compute work to.
         * @param transferQueue The queue the graph will submit transfer work to.
         * @return Returns the GraphicsSubmitInfo used to render the final result of the graph.
         */
        GraphicsSubmitInfo execute(GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue);

    private:
        GhaImage::Format getImageFormat(RgImage image);
        vec2ui getImageSize(RgImage image);
    };
}