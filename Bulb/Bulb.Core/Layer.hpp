#pragma once

#include <Clove/Core/Layer.hpp>

#include <vcclr.h>

namespace Bulb::Core{
	ref class Layer;
}

namespace clv::blb{
	class NativeLayer : public Layer{
		//VARIABLES
	private:
		gcroot<Bulb::Core::Layer^> managedLayer;

		//FUNCTIONS
	public:
		NativeLayer(Bulb::Core::Layer^ layer);
		~NativeLayer();

		virtual void onAttach() override;
		virtual void onUpdate(utl::DeltaTime deltaTime) override;
		virtual void onDetach() override;
	};
}

namespace Bulb::Core{
	public ref class Layer abstract{ //TODO: Make an interface
		//FUNCTIONS
	public:
		virtual void onAttach() abstract;
		virtual void onUpdate(float deltaTime) abstract;
		virtual void onDetach() abstract;
	};
}