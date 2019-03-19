#pragma once

#include "Clove/Layer.h"

namespace clv{
	class CLV_API RenderLayer : public Layer{
		//VARIABLES
	private:
		unsigned int shader = 0;

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

		//FUNCTIONS
	public:
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
	};
}