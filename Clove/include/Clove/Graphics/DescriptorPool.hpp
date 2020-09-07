#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class DescriptorSet;
    class DescriptorSetLayout;

    struct DescriptorInfo {
        DescriptorType type;
        uint32_t count; /**< Number of this type to allocate across all sets. */
    };
}

namespace clv::gfx {
    /**
     * @brief Used to allocate DescriptorSets from a DescriptorSetLayout.
     */
    class DescriptorPool {
        //TYPES
    public:
        enum class Flag {
            None,
            FreeDescriptorSet /**< Descriptor sets can be freed */
        };

        struct Descriptor {
            std::vector<DescriptorInfo> poolTypes;
            Flag flag;
            uint32_t maxSets; /**< The maximum amount of sets that can be allocated from this pool. */
        };

        //FUNCTIONS
    public:
        virtual ~DescriptorPool() = default;

        /** 
         * @brief Allocates a descriptor set for each layout provided.
         */
        virtual std::vector<std::shared_ptr<DescriptorSet>> allocateDescriptorSets(const std::vector<std::shared_ptr<DescriptorSetLayout>>& layouts) = 0;
        virtual void freeDescriptorSets(const std::vector<std::shared_ptr<DescriptorSet>>& descriptorSets)                                           = 0;
    };
}