#include "clvpch.hpp"
#include "ShaderBufferObject.hpp"

namespace clv::gfx{
	ShaderBufferObject::ShaderBufferObject() = default;

	ShaderBufferObject::ShaderBufferObject(ShaderBufferObject&& other) = default;

	ShaderBufferObject& ShaderBufferObject::operator=(ShaderBufferObject&& other) = default;

	ShaderBufferObject::~ShaderBufferObject() = default;
}