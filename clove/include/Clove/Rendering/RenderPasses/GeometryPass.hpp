#pragma once

#include <vector>
#include <memory>

namespace garlic::clove {
    class GhaDescriptorSet;
    class GhaGraphicsCommandBuffer;
    class Mesh;
}

namespace garlic::clove {
    /**
     * @brief A GeometryPass is a single graphics operation that is performed on a set of meshes.
     */
    class GeometryPass {
        //TYPES
    public:
        /**
         * @brief Data a GeometryPass will need for a given frame.
         */
        struct FrameData {
            std::vector<std::shared_ptr<GhaDescriptorSet>> meshDescriptorSets{}; /**< Descriptor set for each mesh submitted for the frame. */

            std::shared_ptr<GhaDescriptorSet> viewDescriptorSet{ nullptr };     /**< Descriptor set for view specific data. */
            std::shared_ptr<GhaDescriptorSet> lightingDescriptorSet{ nullptr }; /**< Descriptor set for lighting specific data. */
        };

        /**
         * @brief A single unit of work for a GeometryPass.
         */
        struct Job {
            size_t meshDescriptorIndex{ 0 };
            std::shared_ptr<Mesh> mesh{ nullptr };
        };

        //FUNCTIONS
    public:
        virtual ~GeometryPass() = default;

        virtual void addJob(Job job) = 0;

        virtual void flushJobs(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) = 0;
    };
}