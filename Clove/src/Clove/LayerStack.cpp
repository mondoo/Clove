#include "clvpch.h"
#include "LayerStack.h"

clv::LayerStack::LayerStack(){
}

clv::LayerStack::~LayerStack(){
	for(Layer* layer : layers){
		delete layer;
	}
}

void clv::LayerStack::pushLayer(Layer* layer){
	layers.emplace(layers.begin() + layerInsertIndex, layer);
	++layerInsertIndex;

	layer->onAttach();
}

void clv::LayerStack::popLayer(Layer* layer){
	auto it = std::find(layers.begin(), layers.end(), layer);
	if(it != layers.end()){
		(*it)->onDetach();

		layers.erase(it);
		--layerInsertIndex;
	}
}

void clv::LayerStack::pushOverlay(Layer* overlay){
	layers.emplace_back(overlay);
	overlay->onAttach();
}

void clv::LayerStack::popOverlay(Layer* overlay){
	auto it = std::find(layers.begin(), layers.end(), overlay);
	if(it != layers.end()){
		(*it)->onDetach();
		layers.erase(it);
	}
}
