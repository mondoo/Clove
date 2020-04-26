#pragma once

#include "Bulb/ECS/Component.hpp"

#include <Clove/Input/Keyboard.hpp>
#include <Clove/Input/Mouse.hpp>

namespace blb::ecs {
	enum class InputResponse {
		Ignored,
		Consumed
	};

	struct hashPair {
		template<typename T1, typename T2>
		size_t operator()(const std::pair<T1, T2>& pair) const {
			return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
		}
	};
}

namespace blb::ecs {
	class InputComponent : public Component<InputComponent> {
		friend class InputSystem;

		//TYPES
	public:
		using BindingId = uint64_t;

		using KeyButtonState		= std::pair<clv::Key, clv::Keyboard::Event::Type>;
		using KeyBindingFunction	= std::function<InputResponse()>;

		using MouseButtonState		= std::pair<clv::MouseButton, clv::Mouse::Event::Type>;
		using MouseBindingFunction	= std::function<InputResponse()>;

		//VARIABLES
	private:
		static BindingId nextId;

		std::unordered_map<KeyButtonState, std::unordered_map<BindingId, size_t>, hashPair> keyIdToIndexMap;
		std::unordered_map<KeyButtonState, std::vector<KeyBindingFunction>, hashPair> keyBindings;

		std::unordered_map<MouseButtonState, std::unordered_map<BindingId, size_t>, hashPair> buttonIdToIndexMap;
		std::unordered_map<MouseButtonState, std::vector<MouseBindingFunction>, hashPair> mouseButtonBindings;

		//FUNCTIONS
	public:
		InputComponent();

		InputComponent(const InputComponent& other);
		InputComponent(InputComponent&& other) noexcept;

		InputComponent& operator=(const InputComponent& other);
		InputComponent& operator=(InputComponent&& other) noexcept;

		~InputComponent();

		template<typename FunctionType, typename ObjectType>
		[nodiscard] BindingId bind(KeyButtonState key, FunctionType&& function, ObjectType* object);
		template<typename FunctionType, typename ObjectType>
		[nodiscard] BindingId bind(MouseButtonState button, FunctionType&& function, ObjectType* object);
		
		template<typename FunctionType>
		[nodiscard] BindingId bind(KeyButtonState key, FunctionType&& function);
		template<typename FunctionType>
		[nodiscard] BindingId bind(MouseButtonState button, FunctionType&& function);

		void unbind(BindingId id);
	};
}

#include "InputComponent.inl"