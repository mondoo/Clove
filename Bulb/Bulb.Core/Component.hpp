#pragma once

namespace Bulb::Core{
	public enum class ComponentType{
		Transform,
		Mesh
	};
}

namespace Bulb::Core{
	public ref class Component{
		//VARIABLES
	public:
		//All temp
		System::String^ name = "Test Component";
		ComponentType type;

		//FUNCTIONS
	public:
		Component(ComponentType type);

		ComponentType getType();
	};
}