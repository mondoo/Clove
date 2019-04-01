#pragma once

namespace clv{
	class Event;

	class CLV_API Layer{
		//VARIABLES
	protected:
		std::string debugName;

		//FUNCTIONS
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach(){}
		virtual void onDetach(){}
		virtual void onUpdate(){}
		virtual void onImGuiRender(){}
		virtual void onEvent(Event& e){}

		inline const std::string& getName() const;
	};
}

#include "Clove/Layer.inl"