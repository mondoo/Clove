#include "Clove/Platform/Mac/CloveMac.h"
#include "Clove/Graphics/Metal/MTLShader.hpp"

#include "Clove/Graphics/Metal/ShaderStrings.hpp"

namespace clv::gfx::mtl{
	MTLShader::MTLShader(id<MTLDevice> mtlDevice, const ShaderDescriptor& descriptor)
		: descriptor(descriptor){
		initialise(mtlDevice, descriptor.style);
	}
	
	MTLShader::MTLShader(MTLShader&& other) noexcept = default;
	
	MTLShader& MTLShader::operator=(MTLShader&& other) noexcept = default;
	
	MTLShader::~MTLShader() = default;
	
	const ShaderDescriptor& MTLShader::getDescriptor() const{
		return descriptor;
	}
	
	ShaderReflectionData MTLShader::getReflectionData() const{
		//TODO:
	}
	
	void MTLShader::initialise(id<MTLDevice> mtlDevice, ShaderStyle style){
		NSError* error2 = [[NSError alloc] init];
		id<MTLLibrary> library;
		
		//TODO: Load the library inside each switch statement
		
		switch(style){
			case ShaderStyle::Lit_3D:

				break;

			case ShaderStyle::Unlit_3D:

				break;

			case ShaderStyle::Unlit_2D:
				{
					NSString* librarySource = [NSString stringWithCString:shader_2D.c_str() encoding:[NSString defaultCStringEncoding]];
					library = [mtlDevice newLibraryWithSource:librarySource options:nil error:&error2];
				}
				break;

			case ShaderStyle::RT:
				
				break;

			case ShaderStyle::Font:
				{
					NSString* librarySource = [NSString stringWithCString:shader_Font.c_str() encoding:[NSString defaultCStringEncoding]];
					library = [mtlDevice newLibraryWithSource:librarySource options:nil error:&error2];
				}
				break;

			case ShaderStyle::CubeShadowMap:
				
				break;

			default:
				CLV_ASSERT(false, "Unknown type! {0}", CLV_FUNCTION_NAME);
				break;
		}
		
		vertexShader = [library newFunctionWithName:@"vertexShader"];
		pixelShader = [library newFunctionWithName:@"pixelShader"];
	}
}
