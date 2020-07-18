#include "Clove/Graphics/OpenGL/GLShader.hpp"

#include "Clove/Graphics/ShaderTranspiler.hpp"

namespace clv::gfx::ogl{
	static GLenum getGLShaderStage(const ShaderStage stage){
		switch (stage){
		case ShaderStage::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderStage::Geometry:
			return GL_GEOMETRY_SHADER;
		case ShaderStage::Pixel:
			return GL_FRAGMENT_SHADER;
		default:
			GARLIC_ASSERT(false, "Uknown stage in {0}", GARLIC_FUNCTION_NAME);
			return GL_VERTEX_SHADER;
		}
	}

	static std::string getStringFromShaderStage(GLuint glShaderStage){
		switch (glShaderStage){
		case GL_VERTEX_SHADER:
			return "vertex";
		case GL_FRAGMENT_SHADER:
			return "pixel";
		case GL_GEOMETRY_SHADER:
			return "geometry";
		default:
			GARLIC_ASSERT(false, "Uknown stage in {0}", GARLIC_FUNCTION_NAME);
			return "unkown";
		}
	}

	GLShader::GLShader(std::shared_ptr<GraphicsFactory> factory, ShaderDescriptor descriptor, std::string_view pathToShader) 
		: factory(std::move(factory))
		, descriptor(std::move(descriptor)) {
		compileShader(getGLShaderStage(this->descriptor.stage), ShaderTranspiler::transpileFromFile(pathToShader, this->descriptor.stage, ShaderType::GLSL));
	}

	GLShader::GLShader(std::shared_ptr<GraphicsFactory> factory, ShaderDescriptor descriptor, const char* bytes, const std::size_t size) 
		: factory(std::move(factory))
		, descriptor(std::move(descriptor)) {
		compileShader(getGLShaderStage(this->descriptor.stage), ShaderTranspiler::transpileFromBytes(bytes, size, this->descriptor.stage, ShaderType::GLSL));
	}

	GLShader::GLShader(GLShader&& other) noexcept = default;

	GLShader& GLShader::operator=(GLShader&& other) noexcept = default;

	GLShader::~GLShader(){
		glDeleteShader(shaderID);
	}

	const std::shared_ptr<GraphicsFactory>& GLShader::getFactory() const {
		return factory;
	}

	const ShaderDescriptor& GLShader::getDescriptor() const{
		return descriptor;
	}

	GLuint GLShader::getShaderID() const{
		return shaderID;
	}

	void GLShader::compileShader(GLenum stage, std::string_view source){
		shaderID = glCreateShader(stage);
		const char* src = source.data();
		glShaderSource(shaderID, 1, &src, nullptr);
		glCompileShader(shaderID);

		int32_t result;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE){
			int32_t length;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetShaderInfoLog(shaderID, length, &length, message);
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to compile {0} shader! {1}", getStringFromShaderStage(stage), message);
			delete[] message;
		}
	}
}