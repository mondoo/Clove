#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;

	struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	struct MaterialData{
		alignas(16) float sininess;
	};

	struct SpriteShaderData{
		math::Matrix4f modelProjection;
	};

	struct MeshRenderData{
		math::Matrix4f modelData{}; //Set on shader in renderer????
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> diffTexture;
		std::shared_ptr<Texture> specTexture;

		void bind() const;
	};
	
	struct SpriteRenderData{
		math::Matrix4f modelData{};
		std::shared_ptr<Texture> texture;

		void bind() const;
	};

	class Renderer{
		//VARIABLES
	protected:
		static std::shared_ptr<gfx::ShaderBufferObject<VertexData>> vertSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<MaterialData>> materialSBO;
		static std::shared_ptr<gfx::ShaderBufferObject<SpriteShaderData>> spriteSBO;

		static std::vector<MeshRenderData> meshSubmissionData;
		static std::vector<SpriteRenderData> spriteSubmissionData;

		static std::shared_ptr<VertexBuffer> spriteVBBuffer;
		static std::shared_ptr<IndexBuffer> spriteIBBuffer;
		static std::shared_ptr<Shader> spriteShader;
		static math::Matrix4f spriteProj;

		//FUNCTIONS
	public:
		static void initialise();

		static void beginScene(); //How would I begin scene with the ecs?
		static void endScene();

		static void submitMesh(const MeshRenderData& data);
		static void submitSprite(const SpriteRenderData& data);
	};
}