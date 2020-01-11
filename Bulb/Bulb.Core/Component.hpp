#pragma once

namespace tnc::ecs::_3D{
	class TransformComponent;
}

//All temp
namespace Bulb::Core{
	public ref class Component{
		//VARIABLES
	public: 
		System::String^ name = "Test Component";

		//FUNCTIONS
	public:
		
	};

	public ref class TransformComponent : Component{
		//VARIABLES
	private:
		tnc::ecs::_3D::TransformComponent* component;

	public:
		TransformComponent(tnc::ecs::_3D::TransformComponent* component);

		float getX();
		void setX(float val);

		float getY();
		void setY(float val);

		float getZ();
		void setZ(float val);
	};
}