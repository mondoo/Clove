#include "clvpch.h"
#include "LayerStack.h"
#include "Layer.h"

namespace clv{
	LayerStack::~LayerStack(){
		for(auto layer : layers){
			layer->onDetach();
		}
	}

	void LayerStack::pushLayer(std::shared_ptr<Layer> layer){
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		layer->onAttach();
	}

	void LayerStack::popLayer(std::shared_ptr<Layer> layer){
		auto it = std::find(layers.begin(), layers.end(), layer);
		if(it != layers.end()){
			layers.erase(it);
			--layerInsertIndex;
			(*it)->onDetach();
		}
	}

	void LayerStack::pushOverlay(std::shared_ptr<Layer> overlay){
		overlay->onAttach();
		layers.emplace_back(std::move(overlay));
	}

	void LayerStack::popOverlay(std::shared_ptr<Layer> overlay){
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if(it != layers.end()){
			layers.erase(it);
			(*it)->onDetach();
		}
	}
}