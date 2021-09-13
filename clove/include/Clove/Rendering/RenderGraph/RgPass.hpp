#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

#include <unordered_set>

namespace clove {
    /**
     * @brief Represents an operation a series of resources.
     */
    class RgPass {
        //VARIABLES
    private:
        RgPassId id{ INVALID_PASS_ID };

        //FUNCTIONS
    public:
        RgPass() = delete;
        RgPass(RgPassId id);

        RgPass(RgPass const &other) = delete;
        RgPass(RgPass &&other) noexcept;

        RgPass &operator=(RgPass const &other) = delete;
        RgPass &operator=(RgPass &&other) noexcept;

        ~RgPass();

        inline RgPassId getId() const;

        virtual std::unordered_set<RgResourceId> getInputResources() const = 0;
        virtual std::unordered_set<RgResourceId> getOutputResources() const = 0;
    };
}

#include "RgPass.inl"