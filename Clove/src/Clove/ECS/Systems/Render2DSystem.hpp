#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform2DComponent.hpp"
#include "Clove/ECS/Components/Renderable2DComponent.hpp"

namespace clv::gfx{
	class Renderer;
	class Shader;
	class VertexBuffer;
	class IndexBuffer;
	//template<typename T> class ShaderBufferObject;
	class Bindable;
}

namespace clv::ecs{
	/*struct ShaderData{
		math::Matrix4f modelProjection;
	};*/

	class Render2DSystem : public System<Transform2DComponent, Renderable2DComponent>{
		//VARIABLES
	public:
		static constexpr SystemID ID = 0xfa8540b2; //VS Generated GUID

	private:
		//ShaderData data;
		math::Matrix4f proj;

		std::vector<unsigned int> indices;

		std::vector<std::unique_ptr<gfx::Bindable>> quadBindables;

		//FUNCTIONS
	public:
		Render2DSystem();
		Render2DSystem(const Render2DSystem& other) = delete;
		Render2DSystem(Render2DSystem&& other) noexcept;
		Render2DSystem& operator=(const Render2DSystem& other) = delete;
		Render2DSystem& operator=(Render2DSystem&& other) noexcept;
		virtual ~Render2DSystem();

		virtual void update(float deltaTime) override;
	};
}
