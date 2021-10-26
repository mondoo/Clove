#include "Clove/Graphics/Metal/MetalDescriptorPool.hpp"

#include "Clove/Graphics/Metal/MetalDescriptorSet.hpp"
#include "Clove/Graphics/Metal/MetalDescriptorSetLayout.hpp"
#include "Clove/Graphics/Metal/MetalLog.hpp"

#include <Clove/Cast.hpp>

namespace clove {
    MetalDescriptorPool::BufferPool::BufferPool() = default;
    
    MetalDescriptorPool::BufferPool::~BufferPool() = default;
    
    id<MTLBuffer> MetalDescriptorPool::BufferPool::allocateBuffer(id<MTLDevice> device, size_t size) {
        BufferEntry *foundEntry{ nullptr };
        
        for(BufferEntry &entry : buffers[size]){
            if(entry.isFree) {
                foundEntry = &entry;
                break;
            }
        }
        
        if(foundEntry == nullptr) {
            buffers[size].push_back({});
            foundEntry = &buffers[size].back();
            
            foundEntry->buffer = [device newBufferWithLength:size options:0];
        }
        
        foundEntry->isFree = false;
        return foundEntry->buffer;
    }
    
    void MetalDescriptorPool::BufferPool::freeBuffer(id<MTLBuffer> buffer) {
        for(auto &&[size, bufferVector] : buffers) {
            for(BufferEntry &entry : bufferVector) {
                if(entry.buffer == buffer){
                    entry.isFree = true;
                    break;
                }
            }
        }
    }
    
    void MetalDescriptorPool::BufferPool::reset() {
        for(auto &&[size, bufferVector] : buffers) {
            for(BufferEntry &entry : bufferVector) {
                entry.isFree = true;
            }
        }
    }
    
    MetalDescriptorPool::MetalDescriptorPool(Descriptor descriptor, id<MTLDevice> device)
        : descriptor{ std::move(descriptor) }
        , device{ device } {
    }
    
    MetalDescriptorPool::MetalDescriptorPool(MetalDescriptorPool &&other) noexcept = default;
    
    MetalDescriptorPool &MetalDescriptorPool::operator=(MetalDescriptorPool &&other) noexcept = default;
    
    MetalDescriptorPool::~MetalDescriptorPool() = default;
    
    GhaDescriptorPool::Descriptor const &MetalDescriptorPool::getDescriptor() const {
        return descriptor;
    }
    
    std::unique_ptr<GhaDescriptorSet> MetalDescriptorPool::allocateDescriptorSets(GhaDescriptorSetLayout const *const layout) {
        return std::move(allocateDescriptorSets(std::vector{ layout })[0]);
    }
    
    std::vector<std::unique_ptr<GhaDescriptorSet>> MetalDescriptorPool::allocateDescriptorSets(std::vector<GhaDescriptorSetLayout const *> const &layouts) {
        @autoreleasepool{
            size_t const setNum{ layouts.size() };
            
            std::vector<MetalDescriptorSetLayout const *> metalDescriptorSetLayouts{};
            metalDescriptorSetLayouts.reserve(setNum);
            for(size_t i{ 0 }; i < layouts.size(); ++i) {
                metalDescriptorSetLayouts.push_back(polyCast<MetalDescriptorSetLayout const>(layouts[i]));
            }
            
            std::vector<std::unique_ptr<GhaDescriptorSet>> descriptorSets{};
            descriptorSets.reserve(setNum);
            for(size_t i{ 0 }; i < layouts.size(); ++i) {
                MetalDescriptorSet::ArgumentEncoder vertexEncoder{};
                if(metalDescriptorSetLayouts[i]->getVertexDescriptors().count > 0){
                    vertexEncoder.encoder       = [device newArgumentEncoderWithArguments:metalDescriptorSetLayouts[i]->getVertexDescriptors()];
                    vertexEncoder.backingBuffer = bufferPool.allocateBuffer(device, vertexEncoder.encoder.encodedLength);
                    
                    [vertexEncoder.encoder setArgumentBuffer:vertexEncoder.backingBuffer offset:0];
                }
                
                MetalDescriptorSet::ArgumentEncoder pixelEncoder{};
                if(metalDescriptorSetLayouts[i]->getPixelDescriptors().count > 0){
                    pixelEncoder.encoder       = [device newArgumentEncoderWithArguments:metalDescriptorSetLayouts[i]->getPixelDescriptors()];
                    pixelEncoder.backingBuffer = bufferPool.allocateBuffer(device, pixelEncoder.encoder.encodedLength);
                    
                    [pixelEncoder.encoder setArgumentBuffer:pixelEncoder.backingBuffer offset:0];
                }
                
                MetalDescriptorSet::ArgumentEncoder computeEncoder{};
                if(metalDescriptorSetLayouts[i]->getComputeDescriptors().count > 0){
                    computeEncoder.encoder       = [device newArgumentEncoderWithArguments:metalDescriptorSetLayouts[i]->getComputeDescriptors()];
                    computeEncoder.backingBuffer = bufferPool.allocateBuffer(device, computeEncoder.encoder.encodedLength);
                    
                    [computeEncoder.encoder setArgumentBuffer:computeEncoder.backingBuffer offset:0];
                }
                
                descriptorSets.emplace_back(std::make_unique<MetalDescriptorSet>(std::move(vertexEncoder), std::move(pixelEncoder), std::move(computeEncoder), layouts[i]));
            }
            
            return descriptorSets;
        }
    }
    
    void MetalDescriptorPool::freeDescriptorSets(std::unique_ptr<GhaDescriptorSet> &descriptorSet) {
        auto *const mtlDescriptorSet{ polyCast<MetalDescriptorSet>(descriptorSet.get()) };
        if(mtlDescriptorSet == nullptr) {
            CLOVE_LOG(CloveGhaMetal, LogLevel::Warning, "{0}: Descriptor set provided is nullptr. Buffers might never be freed", CLOVE_FUNCTION_NAME);
            return;
        }

        freeBuffers(*mtlDescriptorSet);

        descriptorSet.reset();
    }
    
    void MetalDescriptorPool::freeDescriptorSets(std::vector<std::unique_ptr<GhaDescriptorSet>> &descriptorSets) {
        for(auto &descriptorSet : descriptorSets) {
            auto *const mtlDescriptorSet{ polyCast<MetalDescriptorSet>(descriptorSet.get()) };
            if(mtlDescriptorSet == nullptr) {
                CLOVE_LOG(CloveGhaMetal, LogLevel::Warning, "{0}: Descriptor set provided is nullptr. Buffers might never be freed", CLOVE_FUNCTION_NAME);
                continue;
            }

            freeBuffers(*mtlDescriptorSet);

            descriptorSet.reset();
        }
    }
    
    void MetalDescriptorPool::reset() {
        bufferPool.reset();
    }

    void MetalDescriptorPool::freeBuffers(MetalDescriptorSet &descriptorSet) {
        if(id<MTLBuffer> vertexBuffer{ descriptorSet.getVertexBuffer() }) {
            bufferPool.freeBuffer(vertexBuffer);
        }
        if(id<MTLBuffer> pixelBuffer{ descriptorSet.getPixelBuffer() }) {
            bufferPool.freeBuffer(pixelBuffer);
        }
        if(id<MTLBuffer> computeBuffer{ descriptorSet.getComputeBuffer() }) {
            bufferPool.freeBuffer(computeBuffer);
        }
    }
}
