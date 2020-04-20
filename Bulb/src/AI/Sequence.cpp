#include "Bulb/AI/Sequence.hpp"

namespace blb::ai {
	Sequence::Sequence() = default;

	Sequence::Sequence(Sequence&& other) noexcept = default;

	Sequence& Sequence::operator=(Sequence&& other) noexcept = default;

	Sequence::~Sequence() = default;

	Task::Status Sequence::activate(clv::utl::DeltaTime deltaTime) {
		for(const auto& child : children) {
			const Status status = child->activate(deltaTime);
			if(status != Status::success) {
				return status;
			}
		}
		return Status::success;
	}
}