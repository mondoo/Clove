#pragma once

namespace tnc{
	class Layer;

	class LayerStack{
		//VARIABLES
	private:
		std::vector<std::shared_ptr<Layer>> layers;

		uint32_t layerInsertIndex = 0;

		//FUNCTIONS
	public:
		LayerStack();
		LayerStack(const LayerStack& other);
		LayerStack(LayerStack&& other) noexcept;

		~LayerStack();

		void pushLayer(std::shared_ptr<Layer> layer);
		void popLayer(std::shared_ptr<Layer> layer);

		void pushOverlay(std::shared_ptr<Layer> overlay);
		void popOverlay(std::shared_ptr<Layer> overlay);

		std::vector<std::shared_ptr<Layer>>::iterator begin();
		std::vector<std::shared_ptr<Layer>>::iterator end();

		LayerStack& operator=(const LayerStack& other);
		LayerStack& operator=(LayerStack&& other) noexcept;
	};
}

