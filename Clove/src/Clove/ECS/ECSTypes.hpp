#pragma once

namespace clv::ecs{
	using EntityID		= unsigned int;
	using ComponentID	= unsigned int;

	//static constexpr uint64_t	INVALID_POOL_INDEX	= std::numeric_limits<uint64_t>::max();
	static constexpr EntityID	INVALID_ENTITY_ID	= 0;
}