#pragma once

namespace blb {
	template<typename T>
	std::vector<T*> toRawVector(const std::vector<std::unique_ptr<T>>& vec) {
		std::vector<T*> out;
		for(auto& elem : vec) {
			out.push_back(elem.get());
		}
		return out;
	}

	template<typename Action>
	class Transition;

	template<typename Action>
	struct StateInitialiser {
		std::vector<std::unique_ptr<Action>> entryActions;
		std::vector<std::unique_ptr<Action>> actions;
		std::vector<std::unique_ptr<Action>> exitActions;

		std::vector<std::unique_ptr<Transition<Action>>> transitions;
	};

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
		//TODO: Ctors

		State(StateInitialiser<Action>&& initialiser){
			entryActions	= std::move(initialiser.entryActions);
			actions			= std::move(initialiser.actions);
			exitActions		= std::move(initialiser.exitActions);

			transitions		= std::move(initialiser.transitions);
		}

		~State() {
			CLV_LOG_DEBUG("State dtor called");
		}

		std::vector<Action*> getEntryActions() {
			return toRawVector(entryActions);
		}
		std::vector<Action*> getActions() {
			return toRawVector(actions);
		}
		std::vector<Action*> getExitActions() {
			return toRawVector(exitActions);
		}

		std::vector<Transition<Action>*> getTransitions() {
			return toRawVector(transitions);
		}
	};

	template<typename Action>
	class Transition {
		//VARIABLES
	private:
		//TODO: custom deleter for unique ptrs so allocators can be used
		std::function<bool()> condition;
		State<Action>* state;
		std::vector<std::unique_ptr<Action>> actions;

		//FUNCTIONS
	public:
		//TODO: Ctors
		Transition() = default;

		~Transition() {
			CLV_LOG_DEBUG("Transition dtor called");
		}

		//NOTE: Having an init function because we have a construction dependency cycle with the ctors
		void init(std::function<bool()> condition, State<Action>* state) {
			this->condition = std::move(condition);
			this->state = state;
		}
		void init(std::function<bool()> condition, State<Action>* state, std::vector<std::unique_ptr<Action>> actions) {
			this->condition = std::move(condition);
			this->state = state;
			this->actions = std::move(actions);
		}

		bool isTriggered() const {
			return condition();
		}

		State<Action>* getState() const {
			return state;
		}

		std::vector<Action*> getActions() const {
			return toRawVector(actions);
		}
	};

	template<typename Action>
	class StateMachine {
		//VARIABLES
	private:
		State<Action>* currentState = nullptr; //TODO: have a way to set the initial state

		//FUNCTIONS
	public:
		//TODO: Ctors

		StateMachine(State<Action>* initialState)
			: currentState(initialState) {
		}

		std::vector<Action*> update() {
			Transition<Action>* triggeredTransition = nullptr;

			for(auto* transition : currentState->getTransitions()) {
				if(transition->isTriggered()) {
					triggeredTransition = transition;
					break;
				}
			}

			std::vector<Action*> actions;
			if(triggeredTransition != nullptr) {
				State<Action>* targetState = triggeredTransition->getState();

				auto stateExitActions = currentState->getExitActions();
				auto transitionActions = triggeredTransition->getActions();
				auto targetStateEnterActions = targetState->getEntryActions();

				actions.insert(actions.end(), stateExitActions.begin(), stateExitActions.end());
				actions.insert(actions.end(), transitionActions.begin(), transitionActions.end());
				actions.insert(actions.end(), targetStateEnterActions.begin(), targetStateEnterActions.end());

				currentState = targetState;
			} else {
				actions = currentState->getActions();
			}

			return actions;
		}
	};
}