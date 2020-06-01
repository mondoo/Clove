#include "Bulb/AI/Sequence.hpp"

namespace blb::ai {
	Sequence::Sequence() = default;

	Sequence::Sequence(Sequence&& other) noexcept = default;

	Sequence& Sequence::operator=(Sequence&& other) noexcept = default;

	Sequence::~Sequence() = default;

	Task::Status Sequence::activate(const clv::utl::DeltaTime deltaTime, BlackBoard& blackboard) {
		for(const auto& child : children) {
			const Status status = child->activate(deltaTime, blackboard);
			if(status != Status::Success) {
				return status;
			}
		}
		return Status::Success;
	}
}