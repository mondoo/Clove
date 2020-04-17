#include "Tunic/AI/Sequence.hpp"

namespace tnc::ai {
	Sequence::Sequence() = default;

	Sequence::Sequence(Sequence&& other) noexcept = default;

	Sequence& Sequence::operator=(Sequence&& other) noexcept = default;

	Sequence::~Sequence() = default;

	Task::Status Sequence::activate() {
		for(const auto& child : children) {
			const Status status = child->activate();
			if(status != Status::success) {
				return status;
			}
		}
		return Status::success;
	}
}