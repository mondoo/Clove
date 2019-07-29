#pragma once

#include "Clove/Utils/DeltaTime.hpp"

namespace clv{
	class Layer{
		//VARIABLES
	protected:
		std::string debugName;

		//FUNCTIONS
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach(){}
		virtual void onDetach(){}
		virtual void onUpdate(utl::DeltaTime deltaTime){}

		const std::string& getName() const;
	};
}