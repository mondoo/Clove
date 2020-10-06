#include "Clove/Graphics/Metal/MTLShader.hpp"

#include "Clove/Graphics/ShaderTranspiler.hpp"

namespace clv::gfx::mtl{
	MTLShader::MTLShader(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, ShaderDescriptor descriptor, std::string_view pathToShader)
		: factory(std::move(factory))
		, descriptor(std::move(descriptor)){
		std::string mslSource = ShaderTranspiler::transpileFromFile(pathToShader, this->descriptor.stage, ShaderType::MSL);
		NSString* nsMslSource = [NSString stringWithCString:mslSource.c_str() encoding:[NSString defaultCStringEncoding]];

		NSError* error = nullptr;
		id<MTLLibrary> lib = [mtlDevice newLibraryWithSource:nsMslSource options:nil error:&error];
		if(error.code != 0){
			for (NSString* key in [error userInfo]) {
				NSString* value = [error userInfo][key];
				GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Error in function '{0}': {1}", GARLIC_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
			}
		}
			
		shader = [lib newFunctionWithName:@"main0"];
			
		[nsMslSource release];
		[error release];
	}

MTLShader::MTLShader(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, ShaderDescriptor descriptor, std::span<const std::byte> sourceBytes)
		: factory(std::move(factory))
		, descriptor(std::move(descriptor)){
		std::string mslSource = ShaderTranspiler::transpileFromBytes(reinterpret_cast<const char*>(sourceBytes.data()), sourceBytes.size_bytes(), this->descriptor.stage, ShaderType::MSL);
		NSString* nsMslSource = [NSString stringWithCString:mslSource.c_str() encoding:[NSString defaultCStringEncoding]];

		NSError* error = nullptr;
		id<MTLLibrary> lib = [mtlDevice newLibraryWithSource:nsMslSource options:nil error:&error];
		if(error.code != 0){
			for (NSString* key in [error userInfo]) {
				NSString* value = [error userInfo][key];
				GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Error in function '{0}': {1}", GARLIC_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
			}
		}
			
		shader = [lib newFunctionWithName:@"main0"];
			
		[nsMslSource release];
		[error release];
	}
	
	MTLShader::MTLShader(MTLShader&& other) noexcept = default;
	
	MTLShader& MTLShader::operator=(MTLShader&& other) noexcept = default;
	
	MTLShader::~MTLShader() = default;

	const std::shared_ptr<GraphicsFactory>& MTLShader::getFactory() const{
		return factory;
	}
	
	const ShaderDescriptor& MTLShader::getDescriptor() const{
		return descriptor;
	}
	
	const id<MTLFunction> MTLShader::getMTLShader() const{
		return shader;
	}
}
