#include "Clove/LayerStack.hpp"

#include "Clove/Layer.hpp"

#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    LayerStack::LayerStack() = default;

    LayerStack::LayerStack(LayerStack const &other) = default;

    LayerStack::LayerStack(LayerStack &&other) noexcept = default;

    LayerStack &LayerStack::operator=(LayerStack const &other) = default;

    LayerStack &LayerStack::operator=(LayerStack &&other) noexcept = default;

    LayerStack::~LayerStack() {
        CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Deconstructing layer stack...");
        for(auto &layer : layers) {
            CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "{0} detached", layer->getName());
            layer->onDetach();
        }
    }

    void LayerStack::pushLayer(std::shared_ptr<Layer> layer) {
        CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Attached layer: {0}", layer->getName());
        layer->onAttach();
        layers.emplace(layers.begin() + layerInsertIndex++, std::move(layer));
    }

    void LayerStack::popLayer(std::shared_ptr<Layer> const &layer) {
        if(auto it = std::find(layers.begin(), layers.end(), layer); it != layers.end()) {
            CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Popped layer: {0}", (*it)->getName());
            (*it)->onDetach();
            layers.erase(it);
            --layerInsertIndex;
        }
    }

    void LayerStack::pushOverlay(std::shared_ptr<Layer> overlay) {
        CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Attached overlay: {0}", overlay->getName());
        overlay->onAttach();
        layers.emplace_back(std::move(overlay));
    }

    void LayerStack::popOverlay(std::shared_ptr<Layer> const &overlay) {
        if(auto it = std::find(layers.begin(), layers.end(), overlay); it != layers.end()) {
            CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Popped overlay: {0}", (*it)->getName());
            (*it)->onDetach();
            layers.erase(it);
        }
    }
}