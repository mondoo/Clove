#include "Stem/LayerStack.hpp"

#include "Stem/Layer.hpp"

#include <Root/Log/Log.hpp>

namespace garlic::inline stem {
    LayerStack::LayerStack() = default;

    LayerStack::LayerStack(LayerStack const &other) = default;

    LayerStack::LayerStack(LayerStack &&other) noexcept = default;

    LayerStack &LayerStack::operator=(LayerStack const &other) = default;

    LayerStack &LayerStack::operator=(LayerStack &&other) noexcept = default;

    LayerStack::~LayerStack() {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "Deconstructing layer stack...");
        for(auto &layer : layers) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "{0} detached", layer->getName());
            layer->onDetach();
        }
    }

    void LayerStack::pushLayer(std::shared_ptr<Layer> layer) {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "Attached layer: {0}", layer->getName());
        layer->onAttach();
        layers.emplace(layers.begin() + layerInsertIndex++, std::move(layer));
    }

    void LayerStack::popLayer(std::shared_ptr<Layer> const &layer) {
        if(auto it = std::find(layers.begin(), layers.end(), layer); it != layers.end()) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "Popped layer: {0}", (*it)->getName());
            (*it)->onDetach();
            layers.erase(it);
            --layerInsertIndex;
        }
    }

    void LayerStack::pushOverlay(std::shared_ptr<Layer> overlay) {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "Attached overlay: {0}", overlay->getName());
        overlay->onAttach();
        layers.emplace_back(std::move(overlay));
    }

    void LayerStack::popOverlay(std::shared_ptr<Layer> const &overlay) {
        if(auto it = std::find(layers.begin(), layers.end(), overlay); it != layers.end()) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "Popped overlay: {0}", (*it)->getName());
            (*it)->onDetach();
            layers.erase(it);
        }
    }
}