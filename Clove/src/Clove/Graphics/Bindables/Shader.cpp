#include "Shader.hpp"

namespace clv::gfx{
	Shader::Shader() = default;

	Shader::Shader(Shader&& other) noexcept = default;

	Shader& Shader::operator=(Shader&& other) noexcept = default;

	Shader::~Shader() = default;
}