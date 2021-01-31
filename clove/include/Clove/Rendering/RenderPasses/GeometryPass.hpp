#pragma once

#include "Clove/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Maths/Matrix.hpp>
#include <memory>
#include <vector>
#include <type_traits>
#include <typeinfo>

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
        using Id = size_t;

        /**
         * @brief Data a GeometryPass will need for a given frame.
         */
        struct FrameData {
            std::vector<std::shared_ptr<GhaDescriptorSet>> meshDescriptorSets{}; /**< Descriptor set for each mesh submitted for the frame. */

            std::shared_ptr<GhaDescriptorSet> viewDescriptorSet{ nullptr };     /**< Descriptor set for view specific data. */
            std::shared_ptr<GhaDescriptorSet> lightingDescriptorSet{ nullptr }; /**< Descriptor set for lighting specific data. */

            //TODO: This is specific to the light passes. Should it be in here?
            mat4f *currentDirLightTransform{ nullptr };

            mat4f *currentPointLightTransform{ nullptr };
            vec3f currentPointLightPosition{};
            float currentPointLightFarPlane{};
        };

        /**
         * @brief A single unit of work for a GeometryPass.
         */
        struct Job {
            size_t meshDescriptorIndex{ 0 }; /**< Index into FrameData::meshDescriptorSets */
            std::shared_ptr<Mesh> mesh{ nullptr }; //TODO: Move into frame data
        };

        //FUNCTIONS
    public:
        virtual ~GeometryPass() = default;

        template<typename GeometryPassType>
        static Id getId(){
            static_assert(std::is_base_of_v<GeometryPass, GeometryPassType>, "Type passed is not derived from GeometryPass!");
            return typeid(GeometryPassType).hash_code();
        };

        /**
         * @brief Adds a job to this pass' queue.
         */
        virtual void addJob(Job job) = 0;

        /**
         * @brief Clears the job queue
         */
        virtual void flushJobs() = 0;

        /**
         * @brief Submits all jobs into the commandBuffer.
         * @param commandBuffer GhaGraphicsCommandBuffer to record commands into.
         * @param frameData Data that describes the current frame.
         */
        virtual void execute(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) = 0;
    };
}