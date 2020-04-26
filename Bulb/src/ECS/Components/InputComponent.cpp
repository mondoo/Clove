#include "Bulb/ECS/Components/InputComponent.hpp"

namespace blb::ecs {
	InputComponent::BindingId InputComponent::nextId = 0;

	InputComponent::InputComponent() = default;

	InputComponent::InputComponent(const InputComponent& other) = default;

	InputComponent::InputComponent(InputComponent&& other) noexcept {
		keyBindings			= std::move(other.keyBindings);
		mouseButtonBindings = std::move(other.mouseButtonBindings);
	}

	InputComponent& InputComponent::operator=(const InputComponent& other) = default;

	InputComponent& InputComponent::operator=(InputComponent&& other) noexcept {
		keyBindings			= std::move(other.keyBindings);
		mouseButtonBindings = std::move(other.mouseButtonBindings);

		return *this;
	}

	InputComponent::~InputComponent() = default;

	void InputComponent::unbind(BindingId id) {
		//std::unordered_map<BindingId, size_t>& idToIndexMap = keyIdToIndexMap[key];
		//if(auto iter = idToIndexMap.find(id); iter != idToIndexMap.end()) {
		//	std::vector<KeyBindingFunction>& keyVector = keyBindings[key];

		//	const size_t index		= iter->second;
		//	const size_t lastIndex	= keyVector.size() - 1;

		//	if(index < lastIndex) {
		//		keyVector[index] = keyVector.back();
		//	}

		//	keyVector.pop_back();
		//	idToIndexMap.erase(id);

		//	//Update the index map so it knows about the moved component
		//	if(index < lastIndex) {
		//		auto movediter = std::find_if(idToIndexMap.begin(), idToIndexMap.end(), [lastIndex](const std::pair<BindingId, size_t>& keyValPair) {
		//			return (keyValPair.second == lastIndex);
		//		});
		//		const BindingId movedId = movediter->first;
		//		idToIndexMap[movedId] = index;
		//	}
		//}
	}
}