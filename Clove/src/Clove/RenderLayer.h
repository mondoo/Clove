#pragma once

#include "Clove/Layer.h"

#include "Clove/Rendering/API/VertexBuffer.h"
#include "Clove/Rendering/API/IndexBuffer.h"
#include "Clove/Rendering/API/VertexArray.h"
#include "Clove/Rendering/API/VertexBufferLayout.h"
#include "Clove/Rendering/API/Shader.h"
#include "Clove/Rendering/API/Texture.h"

#include "Clove/Rendering/Renderer.h"

namespace clv{
	class CLV_API RenderLayer : public Layer{
		//VARIABLES
	private:
		VertexArray va;

		VertexBuffer vb;
		IndexBuffer ib;

		Shader shader;

		Texture texture;

		Renderer renderer;

		float positions[16] = {
			//vertex pos | tex coord
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};

		unsigned int indicies[6] = {
			0, 1, 2,
			2, 3, 0
		};

		int location = -1;

		float r = 0.0f;
		float increment = 0.01f;

		//FUNCTIONS
	public:
		RenderLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
	};
}