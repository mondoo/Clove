#pragma once

namespace clv{
	class Layer;

	class CLV_API LayerStack{
		//VARIABLES
	private:
		std::vector<std::shared_ptr<Layer>> layers;
		
		unsigned int layerInsertIndex = 0;

		//FUNCTIONS
	public:
		~LayerStack();

		void pushLayer(std::shared_ptr<Layer> layer);
		void popLayer(std::shared_ptr<Layer> layer);
		
		void pushOverlay(std::shared_ptr<Layer> overlay);
		void popOverlay(std::shared_ptr<Layer> overlay);

		std::vector<std::shared_ptr<Layer>>::iterator begin(){ return layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end(){ return layers.end(); }
	};
}

