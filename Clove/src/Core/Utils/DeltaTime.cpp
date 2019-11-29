#include "Clove/Core/Utils/DeltaTime.hpp"

namespace clv::utl{
	DeltaTime::DeltaTime() = default;

	DeltaTime::DeltaTime(const DeltaTime& other) = default;

	DeltaTime& DeltaTime::operator=(const DeltaTime& other) = default;

	DeltaTime::DeltaTime(DeltaTime&& other) noexcept = default;

	DeltaTime& DeltaTime::operator=(DeltaTime&& other) noexcept = default;

	DeltaTime::~DeltaTime() = default;

	DeltaTime::DeltaTime(float deltaSeconds)
		: deltaSeconds(deltaSeconds){
	}

	float DeltaTime::getDeltaSeconds() const{
		return deltaSeconds;
	}

	DeltaTime::operator float() const{
		return deltaSeconds;
	}
}