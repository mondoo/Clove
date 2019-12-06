#pragma once

#include <Clove/Core/ECS/Entity.hpp>

namespace clv::ecs::blb{
	struct NativeEntity : public Entity{
		//FUNCTIONS
	public:
		NativeEntity(EntityID entityID, Manager* manager);

		//Temp: For testing adding entities from C#
		void makeCamEnt(int width, int height);
		void makeCubeEnt();
	};
}

namespace Clove{
	public ref struct Entity{
		//VARIABLES
	private:
		clv::ecs::blb::NativeEntity* nativeEntity = nullptr;

		//FUNCTIONS
	public:
		Entity(clv::ecs::blb::NativeEntity* entity);
		!Entity();

		void makeCamEnt(int width, int height);
		void makeCubeEnt();

		//bool isValid() const;

		//EntityID getID() const;

		/*
		TODO: Add / Get / Remove components
		*/
	};
}