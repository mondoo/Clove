#include "Clove/Rendering/RenderPasses/SkinningPass.hpp"

#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"
#include "Clove/Rendering/RenderingConstants.hpp"

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char skinning_c[];
extern "C" const size_t skinning_cLength;

namespace clove {
    SkinningPass::SkinningPass() = default;

    SkinningPass::SkinningPass(SkinningPass &&other) noexcept = default;

    SkinningPass &SkinningPass::operator=(SkinningPass &&other) noexcept = default;

    SkinningPass::~SkinningPass() = default;
    
    GeometryPass::Id SkinningPass::getId() const {
        return getIdOf<SkinningPass>();
    }

    void SkinningPass::execute(RenderGraph &renderGraph, PassData const &passData) {
        //TODO: Cache instead of making every frame
        std::unordered_map<std::string, std::string> shaderIncludes;
        shaderIncludes["Constants.glsl"] = { constants, constantsLength };

        RgComputePass::Descriptor passDescriptor{
            .shader = renderGraph.createShader({ skinning_c, skinning_cLength }, shaderIncludes, "Mesh skinner (compute)", GhaShader::Stage::Compute),
        };
        RgPassId computePass{ renderGraph.createComputePass(std::move(passDescriptor)) };

        for(auto *job : getJobs()) {
            RgBufferId vertCountBuffer{ renderGraph.createBuffer(sizeof(uint32_t)) };
            renderGraph.writeToBuffer(vertCountBuffer, &job->vertexCount, 0, sizeof(uint32_t));

            RgBufferId skinnedBuffer{ renderGraph.createBuffer(job->vertexBufferSize) };

            renderGraph.addComputeSubmission(computePass, RgComputePass::Submission{
                                                              .readUniformBuffers = {
                                                                  RgBufferBinding{
                                                                      .slot   = 0,
                                                                      .buffer = job->matrixPalette,
                                                                      .offset = 0,
                                                                      .size   = job->matrixPaletteSize,
                                                                  },
                                                                  RgBufferBinding{
                                                                      .slot   = 1,
                                                                      .buffer = vertCountBuffer,
                                                                      .offset = 0,
                                                                      .size   = sizeof(uint32_t),
                                                                  },
                                                              },
                                                              .readStorageBuffers = {
                                                                  RgBufferBinding{
                                                                      .slot   = 2,
                                                                      .buffer = job->vertexBuffer,
                                                                      .offset = 0,
                                                                      .size   = job->vertexBufferSize,
                                                                  },
                                                              },
                                                              .writeBuffers = {
                                                                  RgBufferBinding{
                                                                      .slot   = 3,
                                                                      .buffer = skinnedBuffer,
                                                                      .offset = 0,
                                                                      .size   = job->vertexBufferSize,
                                                                  },
                                                              },
                                                              .disptachSize = { (job->vertexCount / AVERAGE_WORK_GROUP_SIZE) + 1, 1, 1 },
                                                          });

            job->vertexBuffer = skinnedBuffer;
        }
    }
}
