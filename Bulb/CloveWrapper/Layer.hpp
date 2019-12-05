#pragma once

#include <Clove/Core/Layer.hpp>

#include <vcclr.h>

namespace Clove{
	ref class Layer;
}

namespace clv::blb{
	class NativeLayer : public Layer{
		//VARIABLES
	private:
		gcroot<Clove::Layer^> managedLayer;

		//FUNCTIONS
	public:
		NativeLayer(Clove::Layer^ layer);
		~NativeLayer();

		virtual void onAttach() override;
		virtual void onUpdate(utl::DeltaTime deltaTime) override;
		virtual void onDetach() override;
	};
}

namespace Clove{
	public ref class Layer abstract{
		//FUNCTIONS
	public:
		virtual void onAttach() abstract;
		virtual void onUpdate(float deltaTime) abstract;
		virtual void onDetach() abstract;
	};
}