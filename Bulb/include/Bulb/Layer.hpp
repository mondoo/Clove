#pragma once

namespace clv{
	struct InputEvent;
}

namespace blb {
	enum class InputResponse {
		Ignored,
		Consumed
	};
}

namespace blb {
	class Layer {
		//VARIABLES
	protected:
		std::string debugName;

		//FUNCTIONS
	public:
		Layer(std::string name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}

		virtual InputResponse onInputEvent(const clv::InputEvent& event) { return InputResponse::Ignored; }
		virtual void onUpdate(const clv::utl::DeltaTime deltaTime) {}

		virtual void onDetach() {}

		const std::string& getName() const;
	};
}