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

	class Condition { //TODO: Have the condition as a template? this way lambdas can be used?
		//FUNCTIONS
	public:
		virtual ~Condition() = 0;

		virtual bool test() /*const*/ = 0;
	};

	template<typename Action>
	class State {
		//TYPES
	public:
		using Actions		= std::vector<std::unique_ptr<Action>>;
		using Transitions	= std::vector<std::unique_ptr<Transition<Action>>>;

		//VARIABLES
	private:
		//TODO: custom deleter for unique ptrs so allocators can be used
		Actions entryActions;
		Actions actions;
		Actions exitActions;

		Transitions transitions;

		//FUNCTIONS
	public:
		//TODO: Ctors

		State() = default;

		~State() {
			CLV_LOG_DEBUG("State dtor called");
		}

		//NOTE: Having an init function because we have a construction dependency cycle with the ctors
		void init(Actions entryActions, Actions actions, Actions exitActions, Transitions transitions){
			this->entryActions = std::move(entryActions);
			this->actions = std::move(actions);
			this->exitActions = std::move(exitActions);
			this->transitions = std::move(transitions);
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
		std::unique_ptr<Condition> condition;
		std::unique_ptr<State<Action>> state;
		std::vector<std::unique_ptr<Action>> actions;

		//FUNCTIONS
	public:
		//TODO: Ctors
		Transition() = default;

		~Transition(){
			CLV_LOG_DEBUG("Transition dtor called");
		}

		//NOTE: Having an init function because we have a construction dependency cycle with the ctors
		void init(std::unique_ptr<Condition> condition, std::unique_ptr<State<Action>> state, std::vector<std::unique_ptr<Action>> actions){
			this->condition	= std::move(condition);
			this->state		= std::move(state);
			this->actions	= std::move(actions);
		}

		bool isTriggered() const {
			condition->test();
		}

		State<Action>* getState() const {
			return state.get();
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

				actions.insert(actions.end(), currentState->getExitActions().begin(), currentState->getExitActions().end());
				actions.insert(actions.end(), triggeredTransition->getActions().begin(), triggeredTransition->getActions().end());
				actions.insert(actions.end(), targetState->getEntryActions().begin(), targetState->getEntryActions().end());

				currentState = targetState;
			} else {
				actions = currentState->getActions();
			}

			return actions;
		}
	};
}