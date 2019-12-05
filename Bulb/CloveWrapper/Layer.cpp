#include "Layer.hpp"

namespace clv::blb{
	NativeLayer::NativeLayer(Clove::Layer^ layer)
		: managedLayer(layer){
	}

	NativeLayer::~NativeLayer() = default;

	void NativeLayer::onAttach(){
		managedLayer->onAttach();
	}

	void NativeLayer::onUpdate(utl::DeltaTime deltaTime){
		managedLayer->onUpdate(deltaTime.getDeltaSeconds());
	}

	void NativeLayer::onDetach(){
		managedLayer->onDetach();
	}
}