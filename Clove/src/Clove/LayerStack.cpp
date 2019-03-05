#include "clvpch.h"
#include "LayerStack.h"

clv::LayerStack::LayerStack(){
	layerInsert = layers.begin();
}

clv::LayerStack::~LayerStack(){
	for(Layer* layer : layers){
		delete layer;
	}
}

void clv::LayerStack::pushLayer(Layer* layer){
	layerInsert = layers.emplace(layerInsert, layer);
}

void clv::LayerStack::popLayer(Layer* layer){
	auto it = std::find(layers.begin(), layers.end(), layer);
	if(it != layers.end()){
		layers.erase(it);
		--layerInsert;
	}
}

void clv::LayerStack::pushOverlay(Layer* overlay){
	layers.emplace_back(overlay);
}

void clv::LayerStack::popOverlay(Layer* overlay){
	auto it = std::find(layers.begin(), layers.end(), overlay);
	if(it != layers.end()){
		layers.erase(it);
	}
}
