#pragma once

namespace tnc::ecs{
	using EntityID		= clv::uint32;
	using ComponentID	= clv::uint64;

	//static constexpr uint64_t	INVALID_POOL_INDEX	= std::numeric_limits<uint64_t>::max();
	static constexpr EntityID	INVALID_ENTITY_ID	= 0;
}