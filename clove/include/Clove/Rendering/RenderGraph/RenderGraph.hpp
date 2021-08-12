#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgComputePass.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"
#include "Clove/Rendering/RenderGraph/RgRenderPass.hpp"
#include "Clove/Rendering/RenderGraph/RgShader.hpp"
#include "Clove/Rendering/RenderGraph/RgTransferPass.hpp"

#include <Clove/Graphics/Descriptor.hpp>
#include <Clove/Graphics/GhaGraphicsQueue.hpp>
#include <Clove/Graphics/GhaShader.hpp>
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
    class GhaGraphicsPipelineObject;
    class GhaComputePipelineObject;
    class GhaSemaphore;
}

namespace garlic::clove {
    /**
     * @brief Tracks how resources are used to create a graph of how to render a frame.
     */
    class RenderGraph {
        //TYPES
    public:
        struct ExecutionInfo {
            std::vector<std::pair<std::shared_ptr<GhaSemaphore>, PipelineStage>> waitSemaphores{}; /**< What semaphores the render graph should wait on for it's final pass. */
            std::vector<std::shared_ptr<GhaSemaphore>> signalSemaphores{};                         /**< What smepahores the final pass of the render graph should signal when completed. */
            std::shared_ptr<GhaFence> signalFence{};                                               /**< The fence the final pass of the render graph should signal. */
        };

    private:
        struct PassDependency {
            RgPassIdType signalPass{};
            RgPassIdType waitPass{};
            std::shared_ptr<GhaSemaphore> semaphore{};
        };

        //VARIABLES
    private:
        RgFrameCache &frameCache;
        RgGlobalCache &globalCache;

        RgResourceIdType nextResourceId{ 1 };
        RgPassIdType nextPassId{ 1 };

        RgResourceIdType outputResource{ INVALID_RESOURCE_ID }; /**< The final output of the graph. */

        std::unordered_map<RgResourceIdType, std::unique_ptr<RgBuffer>> buffers{};
        std::unordered_map<RgResourceIdType, std::unique_ptr<RgImage>> images{};

        std::unordered_map<RgPassIdType, std::unique_ptr<RgRenderPass>> renderPasses{};
        std::unordered_map<RgPassIdType, std::unique_ptr<RgComputePass>> computePasses{};
        std::unordered_map<RgPassIdType, std::unique_ptr<RgTransferPass>> transferPasses{};

        std::unordered_map<RgResourceIdType, std::shared_ptr<GhaShader>> allocatedShaders{};

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
         * @brief Constructs an RgBuffer with the specified size.
         * @details Buffers are created in video memory unless written to from the CPU.
         * @param bufferSize 
         * @return 
         */
        RgResourceIdType createBuffer(size_t const bufferSize);
        /**
         * @brief Constructs an RgBuffer from an existing GhaBuffer. Usefull if wanting to use
         * pre set up buffers (such as vertex / index buffers).
         * @param buffer GhaBuffer to construct from.
         * @param offset The offset into the GhaBuffer that this RgBuffer will view.
         * @param size The size of the GhaBuffer that this RgBuffer will view.
         * @return
         */
        RgResourceIdType createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t const offset, size_t const size);

        /**
         * @brief Write data into this buffer.
         * @details Will transition this buffer from video memory to system memory if requierd.
         * @param data Pointer to data to write.
         * @param offset Offset into the buffer to write to. If the RgBuffer views an offset into a GhaBuffer then it'll be view offset + offset
         * @param size Size of the region in the buffer to write to.
         */
        void writeToBuffer(RgResourceIdType buffer, void const *data, size_t const offset, size_t const size);

        /**
         * @brief Constructs a new RgImage with the specified type and dimensions.
         * @param imageType 
         * @param format
         * @param dimensions 
         * @return 
         */
        RgResourceIdType createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions);
        /**
         * @brief Creates a new RgImage from an existing image view. Usefull
         * if wanting to use pre made images (such as backbuffers) in the render graph.
         * @param ghaImageView 
         * @return 
         */
        RgResourceIdType createImage(std::shared_ptr<GhaImageView> ghaImageView);

        /**
         * @brief Registers a resource as the final output of the graph.
         * @details The graph will be traversed starting from this resource and build
         * the list of passes it will execute bassed on this output's dependencies.
         * @param resource 
         */
        void registerGraphOutput(RgResourceIdType resource);

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
         * @brief Adds a submission to a render pass. Defining a piece of geometry it needs to render.
         * @param renderPass Pass to add the submission to.
         * @param submission Submission for the pass. Encapsulates any input resources required for a single unit of work.
         */
        void addRenderSubmission(RgPassIdType const renderPass, RgRenderPass::Submission submission);

        /**
         * @brief Create a compute pass.
         * @param passDescriptor Descriptor of the pass itself. Contains what shader to run.
         * @return 
         */
        RgPassIdType createComputePass(RgComputePass::Descriptor passDescriptor);
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
        void buildExecutionPasses(std::vector<RgPassIdType> &outPasses, RgResourceIdType resourceId);
        GhaImage::Layout getPreviousLayout(std::vector<RgPassIdType> const &passes, int32_t const currentPassIndex, RgResourceIdType const imageId);

        std::vector<PassDependency> buildDependencies(std::vector<RgPassIdType> const &passes);

        RgResource *getResourceFromId(RgResourceIdType resourceId);
        RgPass *getPassFromId(RgPassIdType passId);

        void generateRenderPassObjects(std::vector<RgPassIdType> const &passes, std::unordered_map<RgPassIdType, std::shared_ptr<GhaRenderPass>> &outRenderPasses, std::unordered_map<RgPassIdType, std::shared_ptr<GhaFramebuffer>> &outFramebuffers, std::unordered_map<RgPassIdType, std::shared_ptr<GhaGraphicsPipelineObject>> &outGraphicsPipelines, std::unordered_map<RgResourceIdType, std::shared_ptr<GhaSampler>> &outSamplers, std::unordered_map<RgPassIdType, std::shared_ptr<GhaDescriptorSetLayout>> &outDescriptorSetLayouts, std::unordered_map<DescriptorType, uint32_t> &totalDescriptorBindingCount, uint32_t &totalDescriptorSets);
        void generateComputePassObjects(std::vector<RgPassIdType> const &passes, std::unordered_map<RgPassIdType, std::shared_ptr<GhaComputePipelineObject>> &outComputePipelines, std::unordered_map<RgPassIdType, std::shared_ptr<GhaDescriptorSetLayout>> &outDescriptorSetLayouts, std::unordered_map<DescriptorType, uint32_t> &totalDescriptorBindingCount, uint32_t &totalDescriptorSets);

        std::unordered_map<RgPassIdType, std::vector<std::shared_ptr<GhaDescriptorSet>>> createDescriptorSets(std::unordered_map<DescriptorType, uint32_t> const &totalDescriptorBindingCount, uint32_t const totalDescriptorSets, std::unordered_map<RgPassIdType, std::shared_ptr<GhaGraphicsPipelineObject>> const &graphicsPipelines, std::unordered_map<RgPassIdType, std::shared_ptr<GhaComputePipelineObject>> &computePipelines);
    };
}