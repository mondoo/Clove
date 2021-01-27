#pragma once

#include "Clove/Graphics/Descriptor.hpp"

#include <cinttypes>
#include <vector>
#include <memory>

namespace garlic::clove {
    class GhaDescriptorSet;
    class GhaDescriptorSetLayout;

    struct DescriptorInfo {
        DescriptorType type;
        uint32_t count; /**< Number of this type to allocate across all DescriptorSets (usually the array size of each binding). */
    };
}

namespace garlic::clove {
    /**
     * @brief Used to allocate DescriptorSets from a GhaDescriptorSetLayout.
     */
    class GhaDescriptorPool {
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
        virtual ~GhaDescriptorPool() = default;

        virtual const Descriptor &getDescriptor() const = 0;

        /** 
         * @brief Allocates a descriptor set for each layout provided.
         */
        virtual std::shared_ptr<GhaDescriptorSet> allocateDescriptorSets(std::shared_ptr<GhaDescriptorSetLayout> const &layout)                            = 0;
        virtual std::vector<std::shared_ptr<GhaDescriptorSet>> allocateDescriptorSets(std::vector<std::shared_ptr<GhaDescriptorSetLayout>> const &layouts) = 0;

        virtual void freeDescriptorSets(std::shared_ptr<GhaDescriptorSet> const &descriptorSet)               = 0;
        virtual void freeDescriptorSets(std::vector<std::shared_ptr<GhaDescriptorSet>> const &descriptorSets) = 0;

        /**
         * @brief Resets this pool freeing all DescriptorSets allocated from it.
         */
        virtual void reset() = 0;
    };
}