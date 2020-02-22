#include "Tunic/LayerStack.hpp"

#include "Tunic/Layer.hpp"

namespace tnc{
	LayerStack::LayerStack() = default;

	LayerStack::LayerStack(const LayerStack& other) = default;

	LayerStack::LayerStack(LayerStack&& other) noexcept = default;

	LayerStack::~LayerStack(){
		CLV_LOG_TRACE("Deconstructing layer stack...");
		for(auto& layer : layers){
			layer->onDetach();
			CLV_LOG_DEBUG("{0} detached", layer->getName());
		}
	}

	void LayerStack::pushLayer(std::shared_ptr<Layer> layer){
		layer->onAttach();
		CLV_LOG_DEBUG("Attached layer: {0}", layer->getName());
		layers.emplace(layers.begin() + layerInsertIndex, std::move(layer));
	}

	void LayerStack::popLayer(const std::shared_ptr<Layer>& layer){
		auto it = std::find(layers.begin(), layers.end(), layer);
		if(it != layers.end()){
			layers.erase(it);
			--layerInsertIndex;
			CLV_LOG_DEBUG("Popped layer: {0}", (*it)->getName());
			(*it)->onDetach();
		}
	}

	void LayerStack::pushOverlay(std::shared_ptr<Layer> overlay){
		overlay->onAttach();
		CLV_LOG_DEBUG("Attached overlay: {0}", overlay->getName());
		layers.emplace_back(std::move(overlay));
	}

	void LayerStack::popOverlay(const std::shared_ptr<Layer>& overlay){
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if(it != layers.end()){
			layers.erase(it);
			CLV_LOG_DEBUG("Popped overlay: {0}", (*it)->getName());
			(*it)->onDetach();
		}
	}

	std::vector<std::shared_ptr<Layer>>::iterator LayerStack::begin(){
		return layers.begin();
	}

	std::vector<std::shared_ptr<Layer>>::iterator LayerStack::end(){
		return layers.end();
	}

	LayerStack& LayerStack::operator=(const LayerStack& other) = default;

	LayerStack& LayerStack::operator=(LayerStack&& other) noexcept = default;
}