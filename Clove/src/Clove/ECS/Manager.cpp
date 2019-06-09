#include "clvpch.hpp"
#include "Manager.hpp"

namespace clv::ecs{
	Manager::Manager() = default;

	Manager::~Manager() = default;

	void Manager::update(float deltaTime){
		//TODO: update the components
	}

	EntityID Manager::createEntity(){
		//TODO:
		return -1;
	}

	void Manager::destroyEntity(EntityID){
		//TODO:
	}
}