#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform3DComponent.hpp"
#include "Clove/ECS/Components/Renderable3DComponent.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	class Renderer;
	class Context;
	enum class API;
}

namespace clv::ecs{
	//TODO: Temp putting these here
	struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct MaterialData{
		alignas(16) float sininess;
	};
	//

	class Render3DSystem : public System<Transform3DComponent, Renderable3DComponent>{
		//VARIABLES
	public:
		static constexpr SystemID ID = 0x1ae78643; //VS generated GUID

	private:
		std::unique_ptr<gfx::Renderer> renderer;

		//TODO: Temp putting these here
		std::unique_ptr<gfx::ShaderBufferObject<VertexData>> vertCB;
		VertexData vData{};

		std::unique_ptr<gfx::ShaderBufferObject<MaterialData>> materialCB;
		MaterialData mData{};
		//

		//FUNCTIONS
	public:
		Render3DSystem();
		Render3DSystem(const Render3DSystem& other) = delete;
		Render3DSystem(Render3DSystem&& other) noexcept;
		Render3DSystem& operator=(const Render3DSystem& other) = delete;
		Render3DSystem& operator=(Render3DSystem&& other) noexcept;
		virtual ~Render3DSystem();

		virtual void update(float deltaTime) override;

		void initialiseRenderer(const gfx::Context& context);
		gfx::Renderer& getRenderer();

	private:
		math::Matrix4f getTransformWorldMatrix(Transform3DComponent* component);
	};
}
