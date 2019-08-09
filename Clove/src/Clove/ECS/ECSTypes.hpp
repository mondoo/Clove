#pragma once

namespace clv::ecs{
	using EntityID		= uint32;
	using ComponentID	= uint32;

	//static constexpr uint64_t	INVALID_POOL_INDEX	= std::numeric_limits<uint64_t>::max();
	static constexpr EntityID	INVALID_ENTITY_ID	= 0;
}