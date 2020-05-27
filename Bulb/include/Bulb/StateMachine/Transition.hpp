namespace blb {
	template<typename Action>
	class State;
}

namespace blb {
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
		Transition();

		Transition(const Transition& other) = delete;
		Transition(Transition&& other) noexcept;

		Transition& operator=(const Transition& other) = delete;
		Transition& operator=(Transition&& other) noexcept;

		~Transition();

		void init(std::function<bool()> condition, State<Action>* state);
		void init(std::function<bool()> condition, State<Action>* state, std::vector<std::unique_ptr<Action>> actions);

		bool isTriggered() const;

		State<Action>* getState() const;
		std::vector<Action*> getActions() const;
	};
}

#include "Transition.inl"