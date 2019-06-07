#include "clvpch.hpp"
#include "RenderSystem.hpp"

namespace clv::ecs{
	void RenderSystem::update(float deltaTime){
		//example function
		for(auto& compTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(compTuple); //pull the comp we want from the vector
			//Renderable etc.

			//Do some funky draw code
		}
	}
}