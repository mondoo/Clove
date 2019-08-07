#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

#include <queue>

namespace clv::gfx{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;
	class RenderTarget;
	class Mesh;

	/*struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};*/

	struct MaterialData{
		alignas(16) float sininess;
	};

	struct SpriteShaderData{
		math::Matrix4f modelProjection;
	};

	struct ViewData{
		math::Matrix4f view;
		math::Matrix4f projection;
	};

	struct ViewPos{
		alignas(16) math::Vector3f pos;
	};

	struct SpriteRenderData{
		math::Matrix4f modelData{};
		std::shared_ptr<Texture> texture;

		void bind() const;
	};

	struct CameraRenderData{
		math::Vector3f position;
		math::Matrix4f lookAt;
		math::Matrix4f projection;
	};

	struct PointLightData{
		alignas(16) math::Vector3f position;

		alignas(16) math::Vector3f ambient;
		alignas(16) math::Vector3f diffuse;
		alignas(16) math::Vector3f specular;

		/*alignas(16)*/ float constant;
		/*alignas(16)*/ float linear;
		/*alignas(16)*/ float quadratic;
	};
	struct PointLightShaderData{
		int numLights = 0;
		PointLightData lights[10];
	};

	class Renderer{
		//VARIABLES
	protected:
		//static std::shared_ptr<gfx::ShaderBufferObject<VertexData>> vertSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> materialSBO; //TODO: MOVE TO MATERIAL
		static std::shared_ptr<gfx::ShaderBufferObject<SpriteShaderData>> spriteSBO;

		static std::shared_ptr<gfx::ShaderBufferObject<ViewData>> viewDataSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> viewPosSBO;

		static std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> lightDataSBO;
		static PointLightShaderData currentLightInfo;

		static std::queue<std::shared_ptr<Mesh>> meshRenderQueue;
		static std::vector<SpriteRenderData> spriteSubmissionData;
		static CameraRenderData cameraSubmissionData;

		static std::shared_ptr<VertexBuffer> spriteVBBuffer;
		static std::shared_ptr<IndexBuffer> spriteIBBuffer;
		static std::shared_ptr<Shader> spriteShader;
		static math::Matrix4f spriteProj;

		static std::shared_ptr<RenderTarget> renderTarget;

		//FUNCTIONS
	public:
		static void initialise();

		static void beginScene(); //How would I begin scene with the ecs?
		static void endScene();

		static void setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget);
		static void removeRenderTarget();

		static void submitMesh(const std::shared_ptr<Mesh>& data);
		static void submitSprite(const SpriteRenderData& data);
		static void setCamera(const CameraRenderData& data);
		static void submitPointLight(const PointLightData& data);

		//Better way to submit the data?
		//-Have a drawable base?
		//--The two components can inherit it or atleast the 3d one can
	};
}