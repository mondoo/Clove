#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

#include <unordered_set>

namespace clove {
    /**
     * @brief Base class for all resources in the graph. Keeps track of used passes
     */
    class RgResource {
        //VARIABLES
    private:
        std::unordered_set<RgPassId> writePasses{};
        std::unordered_set<RgPassId> readPasses{};

        //FUNCTIONS
    public:
        RgResource();

        RgResource(RgResource const &other) = delete;
        RgResource(RgResource &&other) noexcept;

        RgResource &operator=(RgResource const &other) = delete;
        RgResource &operator=(RgResource &&other) noexcept;

        ~RgResource();

        inline void addWritePass(RgPassId pass);
        inline void addReadPass(RgPassId pass);

        inline std::unordered_set<RgPassId> const &getWritePasses() const;
        inline std::unordered_set<RgPassId> const &getReadPasses() const;
    };
}

#include "RgResource.inl"