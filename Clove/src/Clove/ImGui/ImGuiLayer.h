#pragma once

#include "Clove/Layer.h"
#include "Clove/Events/MouseEvent.h"
#include "Clove/Events/KeyEvent.h"

struct GLFWwindow;

namespace clv{
	class CLV_API ImGuiLayer : public Layer{
		//VARIABLES
	private:
		float time = 0.0f;

		GLFWwindow* window;

		//FUNCTIONS
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onEvent(Event& e) override;

	private:
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool onMouseScrolled(MouseScrolledEvent& e);

		bool onKeyPressed(KeyPressedEvent& e);
		bool onKeyReleased(KeyReleasedEvent& e);
		bool onChar(CharEvent& e);
	};
}