namespace blb {
	template<typename Action>
	class Transition;

	template<typename Action>
	struct StateInitialiser {
		std::vector<std::unique_ptr<Action>> entryActions;
		std::vector<std::unique_ptr<Action>> actions;
		std::vector<std::unique_ptr<Action>> exitActions;

		std::vector<std::unique_ptr<Transition<Action>>> transitions;
	};
}

namespace blb {
	template<typename Action>
	class State {
		//VARIABLES
	private:
		//TODO: custom deleter for unique ptrs so allocators can be used
		std::vector<std::unique_ptr<Action>> entryActions;
		std::vector<std::unique_ptr<Action>> actions;
		std::vector<std::unique_ptr<Action>> exitActions;

		std::vector<std::unique_ptr<Transition<Action>>> transitions;

		//FUNCTIONS
	public:
		State() = delete;
		State(StateInitialiser<Action>&& initialiser);

		State(const State& other) = delete;
		State(State&& other) noexcept;

		State& operator=(const State& other) = delete;
		State& operator=(State&& other) noexcept;

		~State();

		std::vector<Action*> getEntryActions();
		std::vector<Action*> getActions();
		std::vector<Action*> getExitActions();

		std::vector<Transition<Action>*> getTransitions();
	};
}

#include "State.inl"