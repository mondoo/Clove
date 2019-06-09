#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::gfx{
	class Bindable;
	class IndexBuffer;
	class Shader;
}

namespace clv::ecs{
	class RenderableComponent : public Component{
		friend class RenderSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x7dfb2c10; //VS Generated GUID

	private:
		std::vector<std::unique_ptr<gfx::Bindable>> bindables;
		gfx::IndexBuffer* indexBuffer = nullptr;
		gfx::Shader* shader = nullptr;

		//FUNCTIONS
	public:
		RenderableComponent();
		RenderableComponent(const RenderableComponent& other) = delete;
		RenderableComponent(RenderableComponent&& other) noexcept;
		RenderableComponent& operator=(const RenderableComponent& other) = delete;
		RenderableComponent& operator=(RenderableComponent&& other) noexcept;
		virtual ~RenderableComponent();

	protected:
		void addBindable(std::unique_ptr<gfx::Bindable> bindable);
		void addIndexBuffer(std::unique_ptr<gfx::IndexBuffer> indexBuffer);
		void addShader(std::unique_ptr<gfx::Shader> shader);
	};
}
