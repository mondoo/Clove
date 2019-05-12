#include "clvpch.hpp"
#include "GLUniform.hpp"

#include "GraphicsAPI/GL/GLException.hpp"

namespace clv{
	namespace gfx{
		GLUniform::GLUniform(GLUniform&& other) noexcept = default;

		GLUniform& clv::gfx::GLUniform::operator=(GLUniform&& other) noexcept = default;

		GLUniform::~GLUniform() = default;

		GLUniform::GLUniform(const std::string& name){
		}

		void GLUniform::bind(Renderer* renderer){
		}

		void GLUniform::unbind(){
		}
	}
}