#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"
#include "Clove/Rendering/RenderGraph/RgSampler.hpp"

#include <Clove/Maths/Vector.hpp>
#include <vector>

namespace clove {
    class RenderGraph;
    class Blackboard;
}

namespace clove {
    /**
     * @brief A GeometryPass is a single graphics operation that is performed on a set of meshes.
     */
    class GeometryPass {
        //TYPES
    public:
        using Id = size_t;

        /**
         * @brief A single unit of work for a GeometryPass.
         * Contains all the information required for a single mesh.
         */
        struct Job {
            RgBufferId vertexBuffer{};
            RgBufferId indexBuffer{};

            uint32_t indexCount{};

            RgBufferId modelBuffer{};
            RgBufferId colourBuffer{};

            size_t modelBufferSize{};
            size_t colourBufferSize{};

            RgImageId diffuseTexture{};
            RgImageId specularTexture{};
            RgSampler materialSampler{};
        };

        /**
         * @brief Global pass data. Contains information on views / shadow maps etc.
         */
        struct PassData {
            //Render Target
            RgImageId renderTarget{};
            RgImageId depthTarget{};

            vec2ui renderTargetSize{};

            //Shadows
            RgImageId directionalShadowMap{};
            RgImageId pointShadowMap{};

            RgSampler shadowMaplSampler{};

            uint32_t directionalLightCount{};
            uint32_t pointLightCount{};

            std::vector<RgBufferId> directionalLightSpaceBuffers{};
            std::vector<RgBufferId> pointLightSpaceBuffers{};

            //Views
            RgBufferId viewUniformBuffer{};

            size_t viewDataSize{};
            size_t viewPositionSize{};

            size_t viewDataOffset{};
            size_t viewPositionOffset{};

            //Lights
            RgBufferId lightsUnfiromBuffer{};

            size_t numLightsSize{};
            size_t dirShadowTransformsSize{};
            size_t lightsSize{};

            size_t numLightsOffset{};
            size_t dirShadowTransformsOffset{};
            size_t lightsOffset{};
        };

        //VARIABLES
    private:
        std::vector<Job *> jobs{};

        //FUNCTIONS
    public:
        GeometryPass();

        GeometryPass(GeometryPass const &other);
        GeometryPass(GeometryPass &&other) noexcept;

        GeometryPass &operator=(GeometryPass const &other);
        GeometryPass &operator=(GeometryPass &&other) noexcept;

        virtual ~GeometryPass();

        template<typename GeometryPassType>
        static Id getId();

        /**
         * @brief Adds a job to this pass' queue.
         */
        inline void addJob(Job *job);

        /**
         * @brief Clears the job queue
         */
        inline void flushJobs();

        /**
         * @brief 
         * @param renderGraph 
         * @param blackBoard Blackboard containing mesh information.
         */
        virtual void execute(RenderGraph &renderGraph, PassData const &passData) = 0;

    protected:
        inline std::vector<Job *> const &getJobs() const;
    };
}

#include "GeometryPass.inl"