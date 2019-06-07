#pragma once

namespace clv::ecs{
	using EntityID		= unsigned int;
	using ComponentID	= unsigned int;

	static constexpr size_t		INVALID_POOL_INDEX = std::numeric_limits<size_t>::max(); //Will be inconsistent on 32 / 64 -- use uint64?
	static constexpr EntityID	INVALID_ENTITY_ID = 0;
}