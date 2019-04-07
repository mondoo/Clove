#pragma once

#include "Clove/Layer.hpp"

namespace clv{
	class ImGuiLayer : public Layer{
		//VARIABLES
	private:
		float time = 0.0f;

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