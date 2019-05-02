#pragma once

namespace clv{
	class Layer;

	class LayerStack{
		//VARIABLES
	private:
		std::vector<std::shared_ptr<Layer>> layers;

		unsigned int layerInsertIndex = 0;

		//FUNCTIONS
	public:
		CLV_API LayerStack();
		CLV_API LayerStack(const LayerStack& other);
		CLV_API LayerStack(LayerStack&& other) noexcept;

		CLV_API ~LayerStack();

		CLV_API void pushLayer(std::shared_ptr<Layer> layer);
		CLV_API void popLayer(std::shared_ptr<Layer> layer);

		CLV_API void pushOverlay(std::shared_ptr<Layer> overlay);
		CLV_API void popOverlay(std::shared_ptr<Layer> overlay);

		CLV_API std::vector<std::shared_ptr<Layer>>::iterator begin();
		CLV_API std::vector<std::shared_ptr<Layer>>::iterator end();

		CLV_API LayerStack& operator=(const LayerStack& other);
		CLV_API LayerStack& operator=(LayerStack&& other) noexcept;
	};
}

