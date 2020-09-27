#include "Bulb/LayerStack.hpp"

#include "Bulb/Layer.hpp"

#include <Clove/Log.hpp>

namespace blb {
    LayerStack::LayerStack() = default;

    LayerStack::LayerStack(const LayerStack& other) = default;

    LayerStack::LayerStack(LayerStack&& other) noexcept = default;

     LayerStack& LayerStack::operator=(const LayerStack& other) = default;

    LayerStack& LayerStack::operator=(LayerStack&& other) noexcept = default;

    LayerStack::~LayerStack() {
        GARLIC_LOG(garlicLogContext, clv::Log::Level::Trace, "Deconstructing layer stack...");
        for(auto& layer : layers) {
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "{0} detached", layer->getName());
            layer->onDetach();
        }
    }

    void LayerStack::pushLayer(std::shared_ptr<Layer> layer) {
        GARLIC_LOG(garlicLogContext, clv::Log::Level::Trace, "Attached layer: {0}", layer->getName());
        layer->onAttach();
        layers.emplace(layers.begin() + layerInsertIndex++, std::move(layer));
    }

    void LayerStack::popLayer(const std::shared_ptr<Layer>& layer) {
        if(auto it = std::find(layers.begin(), layers.end(), layer); it != layers.end()) {
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "Popped layer: {0}", (*it)->getName());
            (*it)->onDetach();
            layers.erase(it);
            --layerInsertIndex;
        }
    }

    void LayerStack::pushOverlay(std::shared_ptr<Layer> overlay) {
        GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "Attached overlay: {0}", overlay->getName());
        overlay->onAttach();
        layers.emplace_back(std::move(overlay));
    }

    void LayerStack::popOverlay(const std::shared_ptr<Layer>& overlay) {
        if(auto it = std::find(layers.begin(), layers.end(), overlay); it != layers.end()) {
            GARLIC_LOG(garlicLogContext, clv::Log::Level::Debug, "Popped overlay: {0}", (*it)->getName());
            (*it)->onDetach();
            layers.erase(it);
        }
    }

    std::vector<std::shared_ptr<Layer>>::iterator LayerStack::begin() {
        return layers.begin();
    }

    std::vector<std::shared_ptr<Layer>>::iterator LayerStack::end() {
        return layers.end();
    }

    std::vector<std::shared_ptr<Layer>>::reverse_iterator LayerStack::rbegin() {
        return layers.rbegin();
    }

    std::vector<std::shared_ptr<Layer>>::reverse_iterator LayerStack::rend() {
        return layers.rend();
    }
}