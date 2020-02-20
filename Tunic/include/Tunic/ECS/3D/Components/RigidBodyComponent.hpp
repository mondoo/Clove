#pragma once

#include "Tunic/ECS/Core/Component.hpp"

namespace tnc::phy{
	class RigidBody;
}

namespace tnc::ecs::_3D{
	class RigidBodyComponent : public Component<_3D::RigidBodyComponent>{
		friend class PhysicsSystem;

		//VARIABLES
	public:
		clv::utl::MultiCastDelegate<void(RigidBodyComponent*)> onBodyCollision;

	private:
		std::unique_ptr<phy::RigidBody> rigidBody;

		//FUNCTIONS
	public:
		RigidBodyComponent();
		RigidBodyComponent(float mass, bool isKinematic, bool respondToCollision, const clv::mth::vec3f& cubeSize);
		RigidBodyComponent(std::unique_ptr<phy::RigidBody> rigidBody);

		RigidBodyComponent(const RigidBodyComponent& other);
		RigidBodyComponent(RigidBodyComponent&& other) noexcept;

		RigidBodyComponent& operator=(const RigidBodyComponent& other);
		RigidBodyComponent& operator=(RigidBodyComponent&& other) noexcept;

		virtual ~RigidBodyComponent();

	private:
		void initialiseRigidBody(phy::RigidBody* body);
	};
}
