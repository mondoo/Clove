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
	EntityID Manager::nextID = 0;

	Manager::Manager(){
		//Order is somewhat important
		systems = {
			std::make_unique<_2D::PhysicsSystem>(),
			std::make_unique<_2D::RenderSystem>(),

			std::make_unique<_3D::PhysicsSystem>(),
			std::make_unique<_3D::RenderSystem>(),
			std::make_unique<_3D::LightSystem>(),
			std::make_unique<_3D::CameraSystem>(),

			std::make_unique<aud::AudioSystem>(),

			std::make_unique<ui::TextSystem>()
		};

		//TODO: This should change when we can add systems dynamically
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
		return { ++nextID, this };
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

	Entity Manager::getEntity(EntityID ID){
		return { ++nextID, this };
	}
}