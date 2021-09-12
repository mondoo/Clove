#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgComputePass.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"
#include "Clove/Rendering/RenderGraph/RgRenderPass.hpp"
#include "Clove/Rendering/RenderGraph/RgSampler.hpp"
#include "Clove/Rendering/RenderGraph/RgShader.hpp"
#include "Clove/Rendering/RenderGraph/RgTransferPass.hpp"

#include <Clove/Graphics/Descriptor.hpp>
#include <Clove/Graphics/GhaGraphicsQueue.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <Clove/Maths/Vector.hpp>
#include <filesystem>
#include <functional>
#include <unordered_map>
#include <vector>

namespace clove {
    class RgFrameCache;
    class RgGlobalCache;

    class GhaFactory;
    class GhaComputeQueue;
    class GhaGraphicsQueue;
    class GhaTransferQueue;
    class GhaGraphicsCommandBuffer;
    class GhaComputeCommandBuffer;
    class GhaTransferCommandBuffer;
    class GhaGraphicsPipelineObject;
    class GhaComputePipelineObject;
    class GhaSemaphore;
}

namespace clove {
    /**
     * @brief Tracks how resources are used to create a graph of how to render a frame.
     */
    class RenderGraph {
        //TYPES
    public:
        struct ExecutionInfo {
            std::vector<std::pair<GhaSemaphore const *, PipelineStage>> waitSemaphores{}; /**< What semaphores the render graph should wait on for it's final pass. */
            std::vector<GhaSemaphore *> signalSemaphores{};                               /**< What smepahores the final pass of the render graph should signal when completed. */
            GhaFence *signalFence{};                                                      /**< The fence the final pass of the render graph should signal. */
        };

    private:
        struct PassDependency {
            RgPassIdType signalPass{};

            RgPassIdType waitPass{};
            PipelineStage waitStage{};

            GhaSemaphore const *semaphore{};
        };

        //VARIABLES
    private:
        RgFrameCache &frameCache;
        RgGlobalCache &globalCache;

        RgResourceIdType nextResourceId{ 1 };
        RgPassIdType nextPassId{ 1 };

        RgResourceIdType outputResource{ INVALID_RESOURCE_ID }; /**< The final output of the graph. */

        //Resources
        std::unordered_map<RgResourceIdType, std::unique_ptr<RgBuffer>> buffers{};
        std::unordered_map<RgResourceIdType, std::unique_ptr<RgImage>> images{};         /**< Images are only used externally. */
        std::unordered_map<RgResourceIdType, std::unique_ptr<RgImageView>> imageViews{}; /**< Used within the graph itself. We need to track each view so we know what array element needs what layout.*/
        std::unordered_map<RgResourceIdType, GhaSampler *> samplers{};
        std::unordered_map<RgResourceIdType, GhaShader *> shaders{};

        //Passes
        std::unordered_map<RgPassIdType, std::unique_ptr<RgRenderPass>> renderPasses{};
        std::unordered_map<RgPassIdType, std::unique_ptr<RgComputePass>> computePasses{};
        std::unordered_map<RgPassIdType, std::unique_ptr<RgTransferPass>> transferPasses{};

        //FUNCTIONS
    public:
        RenderGraph() = delete;
        RenderGraph(RgFrameCache &frameCache, RgGlobalCache &globalCache);

        RenderGraph(RenderGraph const &other)     = delete;
        RenderGraph(RenderGraph &&other) noexcept = delete;

        RenderGraph &operator=(RenderGraph const &other) = delete;
        RenderGraph &operator=(RenderGraph &&other) noexcept = delete;

        ~RenderGraph();

        /**
         * @brief Constructs a buffer with the specified size.
         * @details Buffers are created in video memory unless written to from the CPU.
         * @param bufferSize 
         * @return 
         */
        RgResourceIdType createBuffer(size_t const bufferSize);
        /**
         * @brief Constructs a buffer from an existing GhaBuffer. Useful if wanting to use
         * pre set up buffers (such as vertex / index buffers).
         * @param buffer GhaBuffer to construct from. Note that the render graph will not own the buffer.
         * @param offset The offset into the GhaBuffer that this RgBuffer will view.
         * @param size The size of the GhaBuffer that this RgBuffer will view.
         * @return
         */
        RgResourceIdType createBuffer(GhaBuffer *buffer, size_t const offset, size_t const size);

        /**
         * @brief Constructs an image with the specified type and dimensions.
         * @param imageType 
         * @param format
         * @param dimensions 
         * @param arrayCount How many elements in the image array to create. If type is GhaImage::Type::Cube then it will create an image with arrayCount * 6.
         * @return 
         */
        RgResourceIdType createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions, uint32_t const arrayCount = 1);
        /**
         * @brief Creates an image from an existing image. Useful if wanting to use
         * pre made images (such as backbuffers) in the render graph.
         * @param ghaImage GhaImage to construct from. Note that the render graph will not own the image.
         * @return 
         */
        RgResourceIdType createImage(GhaImage *ghaImage);

        /**
         * @brief Creates a new RgSampler
         * @param descriptor 
         * @return 
         */
        RgSampler createSampler(GhaSampler::Descriptor descriptor);

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

        /**
         * @brief Creates a render pass.
         * @param passDescriptor Descriptor of the pass itself. Contains any output resources and what shaders to run.
         */
        RgPassIdType createRenderPass(RgRenderPass::Descriptor passDescriptor);

        /**
         * @brief Create a compute pass.
         * @param passDescriptor Descriptor of the pass itself. Contains what shader to run.
         * @return 
         */
        RgPassIdType createComputePass(RgComputePass::Descriptor passDescriptor);

        /**
         * @brief Registers a resource as the final output of the graph.
         * @details The graph will be traversed starting from this resource and build
         * the list of passes it will execute bassed on this output's dependencies.
         * @param resource 
         */
        void registerGraphOutput(RgResourceIdType resource);

        /**
         * @brief Write data into this buffer.
         * @details Will transition this buffer from video memory to system memory if requierd.
         * @param data Pointer to data to write.
         * @param offset Offset into the buffer to write to. If the RgBuffer views an offset into a GhaBuffer then it'll be view offset + offset
         * @param size Size of the region in the buffer to write to.
         */
        void writeToBuffer(RgResourceIdType buffer, void const *data, size_t const offset, size_t const size);

        /**
         * @brief Adds a submission to a render pass. Defining a piece of geometry it needs to render.
         * @param renderPass Pass to add the submission to.
         * @param submission Submission for the pass. Encapsulates any input resources required for a single unit of work.
         */
        void addRenderSubmission(RgPassIdType const renderPass, RgRenderPass::Submission submission);

        /**
         * @brief Adds a submission to the compute pass. Defining a dispatch call.
         * @param computePass Pass to add the submission to.
         * @param submission Submission for the pass. Encapsulates any input/output resources required for a single unit of work.
         */
        void addComputeSubmission(RgPassIdType const computePass, RgComputePass::Submission submission);

        /**
         * @brief Executes the RenderGraph. Creating any objects required and submitting commands to the relevant queues.
         * @param info Semaphores/Fences that will be given to the graph's final pass.
         */
        void execute(ExecutionInfo const &info);

    private:
        RgResourceIdType createImageView(RgImage *image, uint32_t const arrayIndex, uint32_t const arrayCount);

        void buildExecutionPasses(std::vector<RgPassIdType> &outPasses, RgResourceIdType resourceId);
        GhaImage::Layout getPreviousLayout(std::vector<RgPassIdType> const &passes, int32_t const currentPassIndex, RgResourceIdType const imageId);

        std::vector<PassDependency> buildDependencies(std::vector<RgPassIdType> const &passes);

        RgResource *getResourceFromId(RgResourceIdType resourceId);
        RgPass *getPassFromId(RgPassIdType passId);

        void generateRenderPassObjects(std::vector<RgPassIdType> const &passes, std::unordered_map<RgPassIdType, GhaRenderPass *> &outRenderPasses, std::unordered_map<RgPassIdType, GhaFramebuffer *> &outFramebuffers, std::unordered_map<RgPassIdType, GhaGraphicsPipelineObject *> &outGraphicsPipelines, std::unordered_map<RgResourceIdType, GhaSampler *> &outSamplers, std::unordered_map<RgPassIdType, GhaDescriptorSetLayout *> &outDescriptorSetLayouts, std::unordered_map<DescriptorType, uint32_t> &totalDescriptorBindingCount, uint32_t &totalDescriptorSets);
        void generateComputePassObjects(std::vector<RgPassIdType> const &passes, std::unordered_map<RgPassIdType, GhaComputePipelineObject *> &outComputePipelines, std::unordered_map<RgPassIdType, GhaDescriptorSetLayout *> &outDescriptorSetLayouts, std::unordered_map<DescriptorType, uint32_t> &totalDescriptorBindingCount, uint32_t &totalDescriptorSets);

        std::unordered_map<RgPassIdType, std::vector<std::unique_ptr<GhaDescriptorSet>>> createDescriptorSets(std::unordered_map<DescriptorType, uint32_t> const &totalDescriptorBindingCount, uint32_t const totalDescriptorSets, std::unordered_map<RgPassIdType, GhaGraphicsPipelineObject *> const &graphicsPipelines, std::unordered_map<RgPassIdType, GhaComputePipelineObject *> &computePipelines);

        void executeGraphicsPass(RgPassIdType passId, GhaGraphicsCommandBuffer &graphicsCommandBufffer, std::unordered_map<RgPassIdType, GhaRenderPass *> const &allocatedRenderPasses, std::unordered_map<RgPassIdType, GhaFramebuffer *> const &allocatedFramebuffers, std::unordered_map<RgPassIdType, GhaGraphicsPipelineObject *> const &allocatedGraphicsPipelines, std::unordered_map<RgResourceIdType, GhaSampler *> const &allocatedSamplers, std::unordered_map<RgPassIdType, std::vector<std::unique_ptr<GhaDescriptorSet>>> const &allocatedDescriptorSets);
        void executeComputePass(RgPassIdType passId, GhaComputeCommandBuffer &computeCommandBufffer, std::unordered_map<RgPassIdType, GhaComputePipelineObject *> const &allocatedComputePipelines, std::unordered_map<RgPassIdType, std::vector<std::unique_ptr<GhaDescriptorSet>>> const &allocatedDescriptorSets);
    };
}