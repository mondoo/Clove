#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

#include <unordered_set>

namespace garlic::clove {
    /**
     * @brief Represents an operation a series of resources.
     */
    class RgPass {
        //VARIABLES
    private:
        PassIdType id{ INVALID_PASS_ID };

        //FUNCTIONS
    public:
        RgPass() = delete;
        RgPass(PassIdType id);

        RgPass(RgPass const &other) = delete;
        RgPass(RgPass &&other) noexcept;

        RgPass &operator=(RgPass const &other) = delete;
        RgPass &operator=(RgPass &&other) noexcept;

        ~RgPass();

        inline PassIdType getId() const;

        virtual std::unordered_set<ResourceIdType> getInputResources() const = 0;
        virtual std::unordered_set<ResourceIdType> getOutputResources() const = 0;
    };
}

#include "RgPass.inl"