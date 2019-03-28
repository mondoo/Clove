#pragma once

#include "Clove/Layer.h"

namespace clv{
	class CLV_API ImGuiLayer : public Layer{
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