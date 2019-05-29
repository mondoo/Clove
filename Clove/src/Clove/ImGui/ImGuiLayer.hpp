#pragma once

#include "Clove/Layer.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv{
	class ImGuiLayer : public Layer{
		//VARIABLES
	private:
		float time = 0.0f;

		gfx::API currentAPI;

		//FUNCTIONS
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;

		void begin();
		void end();
	};
}