#pragma once

#include "Clove/Core.h"
#include "Layer.h"

#include <vector>

namespace clv{
	class CLV_API LayerStack{
		//VARIABLES
	private:
		std::vector<Layer*> layers;
		unsigned int layerInsertIndex = 0;

		//FUNCTIONS
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		
		void pushOverlay(Layer* overlay);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin(){ return layers.begin(); }
		std::vector<Layer*>::iterator end(){ return layers.end(); }
	};
}

