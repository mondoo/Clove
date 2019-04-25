#include "clvpch.hpp"
#include "LayerStack.hpp"

#include "Clove/Layer.hpp"

namespace clv{
	LayerStack::LayerStack() = default;

	LayerStack::LayerStack(const LayerStack& other) = default;

	LayerStack::LayerStack(LayerStack&& other) = default;

	LayerStack::~LayerStack(){
		CLV_TRACE("Deconstructing layer stack...");
		for(auto layer : layers){
			layer->onDetach();
			CLV_INFO("{0} detached", layer->getName());
		}
	}

	void LayerStack::pushLayer(std::shared_ptr<Layer> layer){
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		CLV_INFO("Attached layer: {0}", layer->getName());
		layer->onAttach();
	}

	void LayerStack::popLayer(std::shared_ptr<Layer> layer){
		auto it = std::find(layers.begin(), layers.end(), layer);
		if(it != layers.end()){
			layers.erase(it);
			--layerInsertIndex;
			CLV_INFO("Popped layer: {0}", (*it)->getName());
			(*it)->onDetach();
		}
	}

	void LayerStack::pushOverlay(std::shared_ptr<Layer> overlay){
		overlay->onAttach();
		CLV_INFO("Attached overlay: {0}", overlay->getName());
		layers.emplace_back(std::move(overlay));
	}

	void LayerStack::popOverlay(std::shared_ptr<Layer> overlay){
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if(it != layers.end()){
			layers.erase(it);
			CLV_INFO("Popped overlay: {0}", (*it)->getName());
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

	LayerStack& LayerStack::operator=(LayerStack&& other) = default;
}