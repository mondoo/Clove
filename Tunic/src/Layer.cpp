#include "Tunic/Layer.hpp"

namespace tnc{
	Layer::Layer(std::string name)
		: debugName(std::move(name)){
	}

	const std::string& Layer::getName() const{
		return debugName;
	}
}
