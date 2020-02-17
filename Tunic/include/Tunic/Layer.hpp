#pragma once

#include <Clove/Core/Utils/DeltaTime.hpp>

namespace tnc{
	class Layer{
		//VARIABLES
	protected:
		std::string debugName;

		//FUNCTIONS
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach(){}
		virtual void onUpdate(clv::utl::DeltaTime deltaTime){}
		virtual void onDetach(){}

		const std::string& getName() const;
	};
}