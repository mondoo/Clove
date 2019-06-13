#include "clvpch.hpp"
#include "Layer.hpp"

#include "Clove/Events/Event.hpp"

namespace clv{
	Layer::Layer(const std::string& name)
		: debugName(name){
	}

	const std::string& Layer::getName() const{
		return debugName;
	}
}
