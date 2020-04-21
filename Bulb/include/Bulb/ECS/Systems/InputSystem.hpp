#pragma once

#include "Bulb/ECS/System.hpp"

namespace clv::plt{
	class Window;
}

namespace blb::ecs{
	class InputSystem : public System {
		//VARIABLES
	private:
		std::shared_ptr<clv::plt::Window> window;

		//FUNCTIONS
	public:
		InputSystem() = delete;
		InputSystem(std::shared_ptr<clv::plt::Window> window);

		InputSystem(const InputSystem& other);
		InputSystem(InputSystem&& other) noexcept;

		InputSystem& operator=(const InputSystem& other);
		InputSystem& operator=(InputSystem&& other) noexcept;

		~InputSystem();

		void update(World& world, clv::utl::DeltaTime deltaTime) final;
	};
}