#pragma once

#include "Clove/Layer.h"
#include "Clove/Events/MouseEvent.h"
#include "Clove/Events/KeyEvent.h"
#include "Clove/Events/ApplicationEvent.h"

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