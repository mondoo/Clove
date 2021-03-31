#pragma once

#include <vector>

namespace garlic::clove {
    /**
     * @brief A RenderGraph node. A simple object to create links between resources.
     */
    class RgNode {
        //VARIABLES
    private:
        std::vector<RgNode *> dependencies{};

        //FUNCTIONS
    public:
        RgNode();

        RgNode(RgNode const &other) = delete;
        RgNode(RgNode &&other) noexcept;

        RgNode &operator=(RgNode const &other) = delete;
        RgNode &operator=(RgNode &&other) noexcept;

        ~RgNode();

    protected:
        void addDependecy(RgNode *dependency);
    };
}