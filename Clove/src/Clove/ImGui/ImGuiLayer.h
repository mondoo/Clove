#pragma once

#include "Clove/Layer.h"
#include "Clove/Events/MouseEvent.h"
#include "Clove/Events/KeyEvent.h"
#include "Clove/Events/ApplicationEvent.h"

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
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onMouseScrolledEvent(MouseScrolledEvent& e);

		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);

		bool onWindowResizedEvent(WindowResizeEvent& e);
	};
}