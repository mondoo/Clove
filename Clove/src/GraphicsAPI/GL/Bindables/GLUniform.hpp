#pragma once

#include "Clove/Graphics/Bindable.hpp"

/*
TODO:
Come back to writing this when building the pipeline for real
need to see how everything will fit together before refactoring 
api specifics like the GL uniforms
*/

namespace clv{
	namespace gfx{
		class Renderer;

		class GLUniform : public Bindable{
			//VARIABLES
		private:
			std::string name;

			//FUNCTIONS
		public:
			GLUniform() = delete;
			GLUniform(const GLUniform& other) = delete;
			GLUniform(GLUniform&& other) noexcept;
			GLUniform& operator=(const GLUniform& other) = delete;
			GLUniform& operator=(GLUniform&& other) noexcept;
			~GLUniform();

			GLUniform(const std::string& name);

			//update to set the value?

			virtual void bind(Renderer* renderer) override;
			virtual void unbind() override;
		};
	}
}