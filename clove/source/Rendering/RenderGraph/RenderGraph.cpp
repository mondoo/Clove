#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"
#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"

namespace garlic::clove {
    RenderGraph::RenderGraph(RgFrameCache &frameCache, RgGlobalCache &globalCache)
        : frameCache{ frameCache }
        , globalCache{ globalCache } {}

    RenderGraph::RenderGraph(RenderGraph const &other) = default;

    RenderGraph::RenderGraph(RenderGraph &&other) noexcept = default;

    RenderGraph &RenderGraph::operator=(RenderGraph const &other) = default;

    RenderGraph &RenderGraph::operator=(RenderGraph &&other) noexcept = default;

    RenderGraph::~RenderGraph() = default;

    RgBuffer RenderGraph::createBuffer(size_t bufferSize) {
        RgBuffer const buffer{ nextId++ };

        GhaBuffer::Descriptor &descriptor{ bufferDescriptors[buffer.id] };
        descriptor.size        = bufferSize;
        descriptor.usageFlags  = static_cast<GhaBuffer::UsageMode>(0);//Will be built when executing the graph
        descriptor.sharingMode = SharingMode::Exclusive; //Assume exclusive to begin with
        descriptor.memoryType  = MemoryType::VideoMemory;//Assume video memory until it has been written to from the host

        return buffer;
    }

    RgBuffer RenderGraph::createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t offset, size_t size) {
        RgBuffer const rgBuffer{ nextId++, offset, size };
        allocatedBuffers[rgBuffer.id] = std::move(buffer);

        return rgBuffer;
    }

    void RenderGraph::writeToBuffer(RgBuffer const &buffer, void const *data, size_t const offset, size_t const size) {
        BufferWrite write{
            .data   = std::vector<std::byte>(size),
            .offset = offset,
            .size   = size,
        };
        memcpy(write.data.data(), data, size);
        bufferWrites[buffer.id] = std::move(write);

        if(!allocatedBuffers.contains(buffer.id)) {
            //Update to system memory if a write has been recorded.
            bufferDescriptors[buffer.id].memoryType = MemoryType::SystemMemory;
        }

        operations.emplace_back([this, buffer]() {
            BufferWrite &write{ bufferWrites.at(buffer.id) };
            allocatedBuffers[buffer.id]->write(write.data.data(), buffer.offset + write.offset, write.size);
        });
    }

    RgImage RenderGraph::createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions) {
        RgImage const image{ nextId++ };

        GhaImage::Descriptor &imageDescriptor{ imageDescriptors[image] };
        imageDescriptor.type        = imageType;
        imageDescriptor.usageFlags  = static_cast<GhaImage::UsageMode>(0);//Will be built when executing the graph
        imageDescriptor.dimensions  = dimensions;
        imageDescriptor.format      = format;
        imageDescriptor.sharingMode = SharingMode::Exclusive;//Assume exclusive to beigin with

        return image;
    }

    RgImage RenderGraph::createImage(std::shared_ptr<GhaImageView> ghaImageView) {
        RgImage const image{ nextId++ };
        allocatedImages[image] = std::move(ghaImageView);

        return image;
    }

    RgShader RenderGraph::createShader(std::filesystem::path file, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextId++ };

        return shader;
    }

    RgShader RenderGraph::createShader(std::string source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextId++ };

        return shader;
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