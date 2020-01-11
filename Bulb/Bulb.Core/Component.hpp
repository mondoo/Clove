#pragma once

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
	public:
		float x = 1.1f;
		float y = 2.2f;
		float z = 3.3f;
	};
}