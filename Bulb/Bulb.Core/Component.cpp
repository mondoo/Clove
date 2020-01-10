#include "Component.hpp"

namespace Bulb::Core{
	Component::Component(ComponentType type)
		: type(type){
	}

	ComponentType Component::getType(){
		return type;
	}
}