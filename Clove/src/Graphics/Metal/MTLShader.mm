#include "Clove/Graphics/Metal/MTLShader.hpp"

#include "Clove/Graphics/Core/ShaderTranspiler.hpp"

namespace clv::gfx::mtl{
	MTLShader::MTLShader(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, const ShaderDescriptor& descriptor, std::string_view pathToShader)
		: factory(std::move(factory))
		, descriptor(descriptor){
		std::string mslSource = ShaderTranspiler::transpileFromFile(pathToShader, descriptor.stage, ShaderType::MSL);
		NSString* nsMslSource = [NSString stringWithCString:mslSource.c_str() encoding:[NSString defaultCStringEncoding]];

		NSError* error = nullptr;
		id<MTLLibrary> lib = [mtlDevice newLibraryWithSource:nsMslSource options:nil error:&error];
		if(error.code != 0){
			for (NSString* key in [error userInfo]) {
				NSString* value = [error userInfo][key];
				CLV_LOG_ERROR("Error in function '{0}': {1}", CLV_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
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
