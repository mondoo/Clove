#include "Bulb/AI/Selector.hpp"

namespace blb::ai {
	Selector::Selector() = default;

	Selector::Selector(Selector&& other) noexcept = default;

	Selector& Selector::operator=(Selector&& other) noexcept = default;

	Selector::~Selector() = default;

	Task::Status Selector::activate(clv::utl::DeltaTime deltaTime) {
		for(const auto& child : children) {
			const Status status = child->activate(deltaTime);
			if(status != Status::Failure) {
				return status;
			}
		}
		return Status::Failure;
	}
}