#include "clvpch.hpp"
#include "Manager.hpp"

namespace clv::ecs{
	Manager::Manager(){
	}

	Manager::~Manager(){
	}

	void Manager::update(float deltaTime){
	}

	EntityID Manager::createEntity(){
		return -1;
	}

	void Manager::destroyEntity(EntityID){
	}
}