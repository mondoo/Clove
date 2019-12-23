#include "Entity.hpp"

namespace Bulb::Core{
	array<Component^>^ Entity::getComponents(){
		array<Component^>^ ret = gcnew array<Component^>(1);
		ret[0] = gcnew Component();
		return ret;
	}
}