#pragma once

namespace blb {
	class Transition;
	class State;

	class Action {
		//VARIABLES
	private:
		//FUNCTIONS
	public:
	};

	class Condition {
		//VARIABLES
	private:
		//FUNCTIONS
	public:
		virtual ~Condition() = 0;

		virtual bool test() const = 0;
	};

	class State {
		//VARIABLES
	private:
		std::vector<std::unique_ptr<Transition>> transitions;

		//FUNCTIONS
	public:
		//TODO: Ctors
		//virtual ~State() = default;

		std::vector<Action*> getEntryActions() {
		}
		std::vector<Action*> getActions() {
		}
		std::vector<Action*> getExitActions() {
		}

		std::vector<Transition*> getTransitions() {
		}
	};

	class Transition {
		//VARIABLES
	private:
		std::unique_ptr<Condition> condition;

		//FUNCTIONS
	public:
		//virtual ~Transition() = default;

		bool isTriggered() const {
			condition->test();
		}

		State* getState() const {
		}

		std::vector<Action*> getActions() const {
		}
	};

	class StateMachine {
		//VARIABLES
	private:
		State* currentState = nullptr; //TODO: have a way to set the initial state

		//FUNCTIONS
	public:
		//TODO: Ctors

		std::vector<Action*> update() {
			Transition* triggeredTransition = nullptr;

			for(auto* transition : currentState->getTransitions()) {
				if(transition->isTriggered()) {
					triggeredTransition = transition;
					break;
				}
			}

			std::vector<Action*> actions;
			if(triggeredTransition != nullptr) {
				State* targetState = triggeredTransition->getState();
				
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