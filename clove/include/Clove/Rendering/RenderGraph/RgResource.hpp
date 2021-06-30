#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

#include <unordered_set>

namespace garlic::clove {
    /**
     * @brief Base class for all resources in the graph. Keeps track of used passes
     */
    class RgResource {
        //VARIABLES
    private:
        ResourceIdType id{ INVALID_RESOURCE_ID };

        std::unordered_set<PassIdType> writePasses{};
        std::unordered_set<PassIdType> readPasses{};

        //FUNCTIONS
    public:
        RgResource() = delete;
        RgResource(ResourceIdType id);

        RgResource(RgResource const &other) = delete;
        RgResource(RgResource &&other) noexcept;

        RgResource &operator=(RgResource const &other) = delete;
        RgResource &operator=(RgResource &&other) noexcept;

        ~RgResource();

        inline void addWritePass(PassIdType pass);
        inline void addReadPass(PassIdType pass);

        inline ResourceIdType getId() const;

        inline std::unordered_set<PassIdType> const &getWritePasses() const;
        inline std::unordered_set<PassIdType> const &getReadPasses() const;
    };
}

#include "RgResource.inl"