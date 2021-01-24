#include "Clove/Rendering/RenderPasses/ColourPass.hpp"

#include "Clove/Rendering/RenderingHelpers.hpp"
#include "Clove/Rendering/Vertex.hpp"

#include <Clove/Graphics/GhaFactory.hpp>
#include <Clove/Graphics/GhaGraphicsCommandBuffer.hpp>
#include <Clove/Graphics/GhaPipelineObject.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char staticmesh_v[];
extern "C" const size_t staticmesh_vLength;
extern "C" const char animatedmesh_v[];
extern "C" const size_t animatedmesh_vLength;

extern "C" const char mesh_p[];
extern "C" const size_t mesh_pLength;

namespace garlic::clove {
    ColourPass::ColourPass(GhaFactory &ghaFactory, std::shared_ptr<GhaRenderPass> ghaRenderPass) {
        //Pipeline
        std::unordered_map<std::string, std::string> shaderIncludes;
        shaderIncludes["Constants.glsl"] = { constants, constantsLength };

        //Create attributes for non-animated meshes
        size_t constexpr totalAttributes{ 6 };
        std::vector<VertexAttributeDescriptor> vertexAttributes{};
        vertexAttributes.reserve(totalAttributes);

        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 0,
            .format   = VertexAttributeFormat::R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, position),
        });
        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 1,
            .format   = VertexAttributeFormat::R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, normal),
        });
        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 2,
            .format   = VertexAttributeFormat::R32G32_SFLOAT,
            .offset   = offsetof(Vertex, texCoord),
        });
        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 3,
            .format   = VertexAttributeFormat::R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, colour),
        });

        AreaDescriptor viewScissorArea{
            .state = ElementState::Dynamic,
        };

        GhaPipelineObject::Descriptor pipelineDescriptor{
            .vertexShader         = *ghaFactory->createShaderFromSource({ staticmesh_v, staticmesh_vLength }, shaderIncludes, "Static Mesh (vertex)", GhaShader::Stage::Vertex),
            .fragmentShader       = *ghaFactory->createShaderFromSource({ mesh_p, mesh_pLength }, shaderIncludes, "Mesh (pixel)", GhaShader::Stage::Pixel),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = vertexAttributes,
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .renderPass           = std::move(ghaRenderPass),
            .descriptorSetLayouts = {
                createMeshDescriptorSetLayout(ghaFactory),
                createViewDescriptorSetLayout(ghaFactory),
                createLightingDescriptorSetLayout(ghaFactory),
            },
            .pushConstants = {},
        };

        pipeline = *ghaFactory->createPipelineObject(pipelineDescriptor);

        //Modify the pipeline for animated meshes
        /* vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 4,
            .format   = VertexAttributeFormat::R32G32B32A32_SINT,
            .offset   = offsetof(Vertex, jointIds),
        });
        vertexAttributes.emplace_back(VertexAttributeDescriptor{
            .location = 5,
            .format   = VertexAttributeFormat::R32G32B32A32_SFLOAT,
            .offset   = offsetof(Vertex, weights),
        });

        pipelineDescriptor.vertexShader     = *ghaFactory->createShaderFromSource({ animatedmesh_v, animatedmesh_vLength }, shaderIncludes, "Animated Mesh (vertex)", GhaShader::Stage::Vertex);
        pipelineDescriptor.vertexAttributes = std::move(vertexAttributes);

        animatedMeshPipeline = *ghaFactory->createPipelineObject(pipelineDescriptor); */
    }

    void ColourPass::addJob(Job job) {
        jobs.emplace_back(std::move(job));
    }

    void ColourPass::flushJobs(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) {
        commandBuffer.bindPipelineObject(*pipeline);

        commandBuffer.bindDescriptorSet(*frameData.viewDescriptorSet, 1);
        commandBuffer.bindDescriptorSet(*frameData.lightingDescriptorSet, 2);

        for(auto &job : jobs) {
            commandBuffer->bindDescriptorSet(*frameData.meshDescriptorSets[job.meshDescriptorIndex], 0);

            commandBuffer.bindVertexBuffer(*job.mesh->getGhaBuffer(), job.mesh->.getVertexOffset());
            commandBuffer.bindIndexBuffer(*job.mesh->.getGhaBuffer(), job.mesh->.getIndexOffset(), IndexType::Uint16);

            commandBuffer.drawIndexed(job.mesh->.getIndexCount());
        }

        jobs.clear();

        //Animated
        /* commandBuffer.bindPipelineObject(*animatedMeshPipeline);

        commandBuffer.bindDescriptorSet(*frameData.viewDescriptorSet, 1);
        commandBuffer.bindDescriptorSet(*frameData.lightingDescriptorSet, 2);

        for(auto &animatedMesh : frameData.animatedMeshes) {
            //TODO: Mesh descriptor set
            //commandBuffer->bindDescriptorSet(*meshSets[index], 0);

            commandBuffer.bindVertexBuffer(*animatedMesh.getGhaBuffer(), animatedMesh.getVertexOffset());
            commandBuffer.bindIndexBuffer(*animatedMesh.getGhaBuffer(), animatedMesh.getIndexOffset(), IndexType::Uint16);

            commandBuffer.drawIndexed(animatedMesh.getIndexCount());
        } */
    }
}