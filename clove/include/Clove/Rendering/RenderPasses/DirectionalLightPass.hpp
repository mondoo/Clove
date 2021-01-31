#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace garlic::clove {
    class GhaFactory;
    class GhaPipelineObject;
    class GhaRenderPass;
}

namespace garlic::clove {
    class DirectionalLightPass : public GeometryPass {
        //VARIABLES
    private:
        std::unique_ptr<GhaPipelineObject> pipeline{ nullptr };

        std::vector<Job> jobs{};

        //FUNCTIONS
    public:
        DirectionalLightPass() = delete;
        DirectionalLightPass(GhaFactory &ghaFactory, std::shared_ptr<GhaRenderPass> ghaRenderPass);//TEMP: Using an external render pass for now but these pass will need to create their own

        DirectionalLightPass(DirectionalLightPass const &other) = delete;
        DirectionalLightPass(DirectionalLightPass &&other) noexcept;

        DirectionalLightPass &operator=(DirectionalLightPass const &other) = delete;
        DirectionalLightPass &operator=(DirectionalLightPass &&other) noexcept;

        ~DirectionalLightPass();

        void addJob(Job job) override;
        void flushJobs() override;

        void execute(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) override;
    };
}