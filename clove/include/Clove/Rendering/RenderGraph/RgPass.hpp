#pragma once

#include <unordered_set>

namespace clove {
    /**
     * @brief Represents an operation a series of resources.
     */
    class RgPass {
        //FUNCTIONS
    public:
        virtual ~RgPass() = default;

        virtual std::unordered_set<RgResourceId> getInputResources() const  = 0;
        virtual std::unordered_set<RgResourceId> getOutputResources() const = 0;
    };
}
