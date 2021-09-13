#include "Clove/Rendering/RenderGraph/RgComputePass.hpp"

namespace clove {
    RgComputePass::RgComputePass(RgPassId id, Descriptor descriptor)
        : RgPass{ id }
        , descriptor{ std::move(descriptor) } {
    }

    RgComputePass::RgComputePass(RgComputePass &&other) noexcept = default;

    RgComputePass &RgComputePass::operator=(RgComputePass &&other) noexcept = default;

    RgComputePass::~RgComputePass() = default;

    std::unordered_set<RgResourceId> RgComputePass::getInputResources() const {
        std::unordered_set<RgResourceId> inputs{};
        for(auto const &submission : submissions) {
            for(auto const &binding : submission.readUniformBuffers) {
                inputs.emplace(binding.buffer);
            }
            for(auto const &binding : submission.readStorageBuffers) {
                inputs.emplace(binding.buffer);
            }
        }
        return inputs;
    }

    std::unordered_set<RgResourceId> RgComputePass::getOutputResources() const {
        std::unordered_set<RgResourceId> outputs{};
        for(auto const &submission : submissions) {
            for(auto const &binding : submission.writeBuffers) {
                outputs.emplace(binding.buffer);
            }
        }
        return outputs;
    }
}