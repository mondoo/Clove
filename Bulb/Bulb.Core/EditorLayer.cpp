#include "EditorLayer.hpp"

#include <Clove/Core/Platform/Application.hpp>
#include <Clove/Core/ECS/Manager.hpp>

namespace clv::blb{
	void EditorLayer::onAttach(){
		
	}

	void EditorLayer::onUpdate(utl::DeltaTime deltaTime){
		
	}

	void EditorLayer::onDetach(){
		auto& ecsManager = clv::plt::Application::get().getManager();

		for(auto& entity : entities){
			ecsManager.destroyEntity(entity.getID());
		}
	}
}