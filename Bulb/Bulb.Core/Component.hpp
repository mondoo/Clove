#pragma once

namespace tnc::ecs::_3D{
	class TransformComponent;
}

//TODO: Create an offcial wrapper for vec3
namespace Bulb::Core{
	public ref struct vec3{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		vec3() : x(0), y(0), z(0){}
		vec3(float x, float y, float z) : x(x), y(y), z(z){}

		vec3(const vec3^& other){
			x = other->x;
			y = other->y;
			z = other->z;
		}
		vec3^ operator=(const vec3^& other){
			x = other->x;
			y = other->y;
			z = other->z;

			return this;
		}
	};

	public ref struct Transform{
		vec3^ position;
		vec3^ rotation;
		vec3^ scale;

		Transform(){
			position	= gcnew vec3();
			rotation	= gcnew vec3();
			scale		= gcnew vec3();
		}
		Transform(vec3^ position, vec3^ rotation, vec3^ scale){
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;
		}
	};
}


//All temp
namespace Bulb::Core{
	public delegate void VecDelegate(vec3^);
	public delegate void TransformDelegate(Transform^);

	public ref class Component{
		//VARIABLES
	public: 
		System::String^ name = "Test Component";

		//FUNCTIONS
	public:
		
	};

	public ref class TransformComponent : Component{
		//VARIABLES
	public:
		event TransformDelegate^ onTransformChanged;

	private:
		tnc::ecs::_3D::TransformComponent* component;

	public:
		TransformComponent(tnc::ecs::_3D::TransformComponent* component);

		vec3^ getPosition();
		void setPosition(vec3^ position);

		vec3^ getRotation();
		void setRotation(vec3^ rotation);

		vec3^ getScale();
		void setScale(vec3^ scale);
	};
}