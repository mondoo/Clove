#pragma once

namespace blb {
	class Brain {
		//FUNCTIONS
	public:
		virtual ~Brain() = default;

		virtual void update(const clv::utl::DeltaTime deltaTime) = 0;
	};
}