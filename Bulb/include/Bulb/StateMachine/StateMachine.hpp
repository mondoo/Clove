#pragma once

namespace blb{
	template<typename Action>
	class State;
}

namespace blb {
	template<typename Action>
	class StateMachine {
		//VARIABLES
	private:
		State<Action>* currentState = nullptr;

		//FUNCTIONS
	public:
		StateMachine() = delete;
		StateMachine(State<Action>* initialState);

		StateMachine(const StateMachine& other) = delete;
		StateMachine(StateMachine&& other) noexcept;

		StateMachine& operator=(const StateMachine& other) = delete;
		StateMachine& operator=(StateMachine&& other) noexcept;

		~StateMachine();

		std::vector<Action*> update();
	};
}

#include "StateMachine.inl"