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
		//FUNCTIONS
	public:
		//TODO: Ctors

		void update() {
		}
	};
}