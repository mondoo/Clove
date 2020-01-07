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
	
	const id<MTLFunction> MTLShader::getMTLVertexShader() const{
		return vertexShader;
	}
	
	const id<MTLFunction> MTLShader::getMTLPixelShader() const{
		return pixelShader;
	}
	
	const ShaderDescriptor& MTLShader::getDescriptor() const{
		return descriptor;
	}
	
	ShaderReflectionData MTLShader::getReflectionData() const{
		ShaderReflectionData outData;
		
		NSArray<MTLVertexAttribute*>* attributes = vertexShader.vertexAttributes;
		
		for(NSInteger i = 0; i < attributes.count; ++i){
			outData.vertexBufferLayout.add(VertexElement::getTypeFromSemantic([attributes[i].name cStringUsingEncoding:[NSString defaultCStringEncoding]]));
		}
		
		return outData;
	}
	
	void MTLShader::initialise(id<MTLDevice> mtlDevice, ShaderStyle style){
		NSError* error;
		id<MTLLibrary> library;
		
		//TODO: Load the library inside each switch statement
		
		switch(style){
			case ShaderStyle::Lit_3D:
				CLV_DEBUG_BREAK;
				break;

			case ShaderStyle::Unlit_3D:
				{
					NSString* librarySource = [NSString stringWithCString:shader_Unlit.c_str() encoding:[NSString defaultCStringEncoding]];
					library = [mtlDevice newLibraryWithSource:librarySource options:nil error:&error];
				}
				break;

			case ShaderStyle::Unlit_2D:
				{
					NSString* librarySource = [NSString stringWithCString:shader_2D.c_str() encoding:[NSString defaultCStringEncoding]];
					library = [mtlDevice newLibraryWithSource:librarySource options:nil error:&error];
				}
				break;

			case ShaderStyle::RT:
				CLV_DEBUG_BREAK;
				break;

			case ShaderStyle::Font:
				{
					NSString* librarySource = [NSString stringWithCString:shader_Font.c_str() encoding:[NSString defaultCStringEncoding]];
					library = [mtlDevice newLibraryWithSource:librarySource options:nil error:&error];
				}
				break;

			case ShaderStyle::CubeShadowMap:
				CLV_DEBUG_BREAK;
				break;

			default:
				CLV_ASSERT(false, "Unknown type! {0}", CLV_FUNCTION_NAME);
				break;
		}
		
		if(error.code != 0){
			for (NSString* key in [error userInfo]) {
				NSString* value = [error userInfo][key];
				CLV_LOG_ERROR("Error in function '{0}': {1}", CLV_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
			}
		}
		
		vertexShader = [library newFunctionWithName:@"vertexShader"];
		pixelShader = [library newFunctionWithName:@"pixelShader"];
	}
}
