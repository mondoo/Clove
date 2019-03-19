#pragma once

#include "Clove/Layer.h"

namespace clv{
	class CLV_API RenderLayer : public Layer{
		//VARIABLES
	private:
		unsigned int bufferID = 0;
		unsigned int ibo = 0;
		unsigned int shader = 0;

		unsigned int vao;

		float positions[8] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,
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
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
	};
}