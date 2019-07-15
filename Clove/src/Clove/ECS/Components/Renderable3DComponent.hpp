#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::gfx{
	class Bindable;
	class IndexBuffer;
	class Shader;
}

namespace clv::ecs{
	class Renderable3DComponent : public Component{
		friend class Render3DSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x7dfb2c10; //VS Generated GUID

	private:
		std::vector<std::shared_ptr<gfx::Bindable>> bindables;
		gfx::IndexBuffer* indexBuffer = nullptr;

		//FUNCTIONS
	public:
		Renderable3DComponent();
		Renderable3DComponent(const Renderable3DComponent& other) = delete;
		Renderable3DComponent(Renderable3DComponent&& other) noexcept;
		Renderable3DComponent& operator=(const Renderable3DComponent& other) = delete;
		Renderable3DComponent& operator=(Renderable3DComponent&& other) noexcept;
		virtual ~Renderable3DComponent();

	protected:
		void addBindable(const std::shared_ptr<gfx::Bindable>& bindable);
		void addIndexBuffer(const std::shared_ptr<gfx::IndexBuffer>& indexBuffer);
	};
}
