#pragma once

namespace garlic::inline stem {
    class Layer;
}

namespace garlic::inline stem {
    class LayerStack {
        //VARIABLES
    private:
        std::vector<std::shared_ptr<Layer>> layers;

        uint32_t layerInsertIndex{ 0 };

        //FUNCTIONS
    public:
        LayerStack();

        LayerStack(LayerStack const &other);
        LayerStack(LayerStack &&other) noexcept;

        LayerStack &operator=(LayerStack const &other);
        LayerStack &operator=(LayerStack &&other) noexcept;

        ~LayerStack();

        void pushLayer(std::shared_ptr<Layer> layer);
        void popLayer(std::shared_ptr<Layer> const &layer);

        void pushOverlay(std::shared_ptr<Layer> overlay);
        void popOverlay(std::shared_ptr<Layer> const &overlay);

        inline std::vector<std::shared_ptr<Layer>>::iterator begin();
        inline std::vector<std::shared_ptr<Layer>>::iterator end();

        inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin();
        inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rend();
    };
}

#include "LayerStack.inl"
