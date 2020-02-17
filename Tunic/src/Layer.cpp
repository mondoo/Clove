#include "Tunic/Layer.hpp"

namespace tnc{
	Layer::Layer(const std::string& name)
		: debugName(name){
	}

	const std::string& Layer::getName() const{
		return debugName;
	}
}
