#pragma once

#include "Clove/Graphics/Descriptor.hpp"

#include <cinttypes>
#include <vector>
#include <memory>

namespace garlic::clove {
    class DescriptorSet;
    class DescriptorSetLayout;

    struct DescriptorInfo {
        DescriptorType type;
        uint32_t count; /**< Number of this type to allocate across all DescriptorSets (usually the array size of each binding). */
    };
}

namespace garlic::clove {
    /**
     * @brief Used to allocate DescriptorSets from a DescriptorSetLayout.
     */
    class DescriptorPool {
        //TYPES
    public:
        enum class Flag {
            None,
            FreeDescriptorSet /**< DescriptorSets can be freed */
        };

        struct Descriptor {
            std::vector<DescriptorInfo> poolTypes;
            Flag flag;
            uint32_t maxSets; /**< The maximum amount of DescriptorSets that can be allocated from this pool. */
        };

        //FUNCTIONS
    public:
        virtual ~DescriptorPool() = default;

        virtual const Descriptor &getDescriptor() const = 0;

        /** 
         * @brief Allocates a descriptor set for each layout provided.
         */
        virtual std::shared_ptr<DescriptorSet> allocateDescriptorSets(std::shared_ptr<DescriptorSetLayout> const &layout)                            = 0;
        virtual std::vector<std::shared_ptr<DescriptorSet>> allocateDescriptorSets(std::vector<std::shared_ptr<DescriptorSetLayout>> const &layouts) = 0;

        virtual void freeDescriptorSets(std::shared_ptr<DescriptorSet> const &descriptorSet)               = 0;
        virtual void freeDescriptorSets(std::vector<std::shared_ptr<DescriptorSet>> const &descriptorSets) = 0;

        /**
         * @brief Resets this pool freeing all DescriptorSets allocated from it.
         */
        virtual void reset() = 0;
    };
}