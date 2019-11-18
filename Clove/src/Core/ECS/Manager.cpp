#include "Manager.hpp"

#include "Core/ECS/2D/Systems/RenderSystem.hpp"
#include "Core/ECS/2D/Systems/PhysicsSystem.hpp"
#include "Core/ECS/3D/Systems/RenderSystem.hpp"
#include "Core/ECS/3D/Systems/PhysicsSystem.hpp"
#include "Core/ECS/3D/Systems/LightSystem.hpp"
#include "Core/ECS/3D/Systems/CameraSystem.hpp"
#include "Core/ECS/Audio/Systems/AudioSystem.hpp"
#include "Core/ECS/UI/Systems/TextSystem.hpp"

namespace clv::ecs{
	EntityID Manager::nextID = 1;

	Manager::Manager(){
		//Order is somewhat important
		systems.reserve(8);
		systems.push_back(std::make_unique<_2D::PhysicsSystem>());
		systems.push_back(std::make_unique<_2D::RenderSystem>());

		systems.push_back(std::make_unique<_3D::PhysicsSystem>());
		systems.push_back(std::make_unique<_3D::RenderSystem>());
		systems.push_back(std::make_unique<_3D::LightSystem>());
		systems.push_back(std::make_unique<_3D::CameraSystem>());

		systems.push_back(std::make_unique<aud::AudioSystem>());

		systems.push_back(std::make_unique<ui::TextSystem>());

		std::for_each(systems.begin(), systems.end(), [this](const std::unique_ptr<System>& system){
			system->manager = this;
		});
	}

	Manager::~Manager() = default;

	void Manager::update(utl::DeltaTime deltaTime){
		std::for_each(systems.begin(), systems.end(), [deltaTime](const std::unique_ptr<System>& system){
			system->update(deltaTime);
		});
	}

	Entity Manager::createEntity(){
		return { nextID++, this };
	}

	Entity Manager::getEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return {};
		} else{
			return { ID, this };
		}
	}

	void Manager::destroyEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return;
		}

		//TODO: Destroy entity

		/*std::for_each(systems.begin(), systems.end(), [ID](const std::unique_ptr<SystemBase>& system){
			system->onEntityDestroyed(ID);
		});*/
		//components.erase(ID);
	}
}