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

        std::unordered_set<ResourceIdType> inputResources{};
        std::unordered_set<ResourceIdType> outputResources{};

        //FUNCTIONS
    public:
        RgPass() = delete;
        RgPass(PassIdType id);

        RgPass(RgPass const &other) = delete;
        RgPass(RgPass &&other) noexcept;

        RgPass &operator=(RgPass const &other) = delete;
        RgPass &operator=(RgPass &&other) noexcept;

        ~RgPass();

        inline void addInputResource(ResourceIdType resource);
        inline void addOutputResource(ResourceIdType resource);

        inline PassIdType getId() const;

        inline std::unordered_set<ResourceIdType> const &getInputResource() const;
        inline std::unordered_set<ResourceIdType> const &getOutputResource() const;
    };
}

#include "RgPass.inl"