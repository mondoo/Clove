#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

namespace garlic::clove {
    RenderGraph::RenderGraph(std::shared_ptr<RgArena> area) {
        //TODO
    };

    RenderGraph::RenderGraph(RenderGraph const &other) = default;

    RenderGraph::RenderGraph(RenderGraph &&other) noexcept = default;

    RenderGraph &RenderGraph::operator=(RenderGraph const &other) = default;

    RenderGraph &RenderGraph::operator=(RenderGraph &&other) noexcept = default;

    RenderGraph::~RenderGraph() = default;

    RgBuffer RenderGraph::createBuffer(size_t bufferSize) {
        //TODO

        return {};
    }

    RgBuffer RenderGraph::createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t offset, size_t size) {
        //TODO

        return {};
    }

    RgImage RenderGraph::createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions) {
        //TODO

        return {};
    }

    RgImage RenderGraph::createImage(std::shared_ptr<GhaImageView> ghaImageView) {
        //TODO

        return {};
    }

    RgShader RenderGraph::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        //TODO

        return {};
    }

    RgShader RenderGraph::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        //TODO

        return {};
    }

    RgGraphicsPipelineState RenderGraph::createGraphicsPipelineState(RgGraphicsPipelineState::Descriptor desciptor) {
        //TODO

        return {};
    }

    RgComputePipelineState RenderGraph::createComputePipelineState(RgComputePipelineState::Descriptor descriptor) {
        //TODO

        return {};
    }

    void RenderGraph::addGraphicsPass(RgGraphicsPipelineState pipelineState, std::vector<RenderGraph::GraphicsSubmission> pass) {
    }

    void RenderGraph::addComputePass(RgComputePipelineState pipelineState, std::vector<RenderGraph::ComputeSubmission> pass) {
        //TODO
    }

    //Return a submit info?
    GraphicsSubmitInfo RenderGraph::execute(GhaFactory &factory, GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue) {
        //TODO

        return {};
    }
}