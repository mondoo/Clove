#pragma once

namespace clv{
	class Event;

	class Layer{
		//VARIABLES
	protected:
		std::string debugName;

		//FUNCTIONS
	public:
		CLV_API Layer(const std::string& name = "Layer");
		CLV_API virtual ~Layer() = default;

		CLV_API virtual void onAttach(){}
		CLV_API virtual void onDetach(){}
		CLV_API virtual void onUpdate(){}
		CLV_API virtual void onImGuiRender(){}
		CLV_API virtual void onEvent(Event& e){}

		CLV_API inline const std::string& getName() const;
	};
}

#include "Clove/Layer.inl"