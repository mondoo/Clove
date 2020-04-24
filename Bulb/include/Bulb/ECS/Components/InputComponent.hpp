#pragma once

#include "Bulb/ECS/Component.hpp"

#include <Clove/Input/Keyboard.hpp>
#include <Clove/Input/Mouse.hpp>

namespace blb::ecs {
	enum class InputResponse {
		ignored,
		consumed
	};
}

namespace blb::ecs {
	class InputComponent : public Component<InputComponent> {
		friend class InputSystem;

		//TYPES
	public:
		using BindingId = uint64_t;
		using KeyBindingFunction	= std::function<InputResponse(const clv::Keyboard::Event&)>;
		using MouseBindingFunction	= std::function<InputResponse(const clv::Mouse::Event&)>;

		//VARIABLES
	private:
		static BindingId nextId;

		std::unordered_map<clv::Key, std::unordered_map<BindingId, size_t>> keyIdToIndexMap;
		std::unordered_map<clv::Key, std::vector<KeyBindingFunction>> keyBindings;

		std::unordered_map<clv::MouseButton, std::unordered_map<BindingId, size_t>> buttonIdToIndexMap;
		std::unordered_map<clv::MouseButton, std::vector<MouseBindingFunction>> mouseButtonBindings;

		//FUNCTIONS
	public:
		InputComponent();

		InputComponent(const InputComponent& other);
		InputComponent(InputComponent&& other) noexcept;

		InputComponent& operator=(const InputComponent& other);
		InputComponent& operator=(InputComponent&& other) noexcept;

		~InputComponent();

		template<typename FunctionType, typename ObjectType>
		[nodiscard] BindingId bind(clv::Key key, FunctionType&& function, ObjectType* object);
		template<typename FunctionType, typename ObjectType>
		[nodiscard] BindingId bind(clv::MouseButton button, FunctionType&& function, ObjectType* object);
		
		template<typename FunctionType>
		[nodiscard] BindingId bind(clv::Key key, FunctionType&& function);
		template<typename FunctionType>
		[nodiscard] BindingId bind(clv::MouseButton button, FunctionType&& function);

		void unbind(clv::Key key, BindingId id);
		void unbind(clv::MouseButton button, BindingId id);
	};
}

#include "InputComponent.inl"