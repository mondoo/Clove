#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

namespace garlic::clove {
    RenderGraph::RenderGraph(RgArena &arena)
        : arena{ arena } {};

    RenderGraph::RenderGraph(RenderGraph const &other) = default;

    RenderGraph::RenderGraph(RenderGraph &&other) noexcept = default;

    RenderGraph &RenderGraph::operator=(RenderGraph const &other) = default;

    RenderGraph &RenderGraph::operator=(RenderGraph &&other) noexcept = default;

    RenderGraph::~RenderGraph() = default;

    RgBuffer RenderGraph::createBuffer(size_t bufferSize) {
        RgBuffer const buffer{ nextId++ };

        GhaBuffer::Descriptor &descriptor{ bufferDescriptors[buffer] };
        descriptor.size       = bufferSize;
        descriptor.memoryType = MemoryType::VideoMemory;//Assume video memory until it has been written to from the host

        return buffer;
    }

    RgBuffer RenderGraph::createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t offset, size_t size) {
        ResourceIdType const id{ nextId++ };
        externalBuffers[id] = std::move(buffer);

        return RgBuffer{ id };
    }

    void RenderGraph::writeToBuffer(RgBuffer const &buffer, void const *data, size_t const offset, size_t const size) {
        BufferWrite write{
            .data   = std::vector<std::byte>(size),
            .offset = offset,
            .size   = size,
        };
        memcpy(write.data.data(), data, size);
        bufferWrites[buffer] = std::move(write);

        if(!externalBuffers.contains(buffer)) {
            //Update to system memory if a write has been recorded.
            bufferDescriptors[buffer].memoryType = MemoryType::SystemMemory;
        }

        operations.emplace_back([this, buffer]() {
            BufferWrite &write{ bufferWrites.at(buffer) };
            allocatedBuffers[buffer]->write(write.data.data(), write.offset, write.size);
        });
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