#include "Clove/Graphics/Metal/MTLShader.hpp"

#include "Clove/Graphics/Metal/ShaderStrings.hpp"
#include "Clove/Graphics/Core/ShaderTranspiler.hpp"

namespace clv::gfx::mtl{
		std::string source_vs = R"(
	cbuffer viewBuffer : register(b6){
		matrix modelProjection;
	};

	struct VSOut{
		float2 tex : TexCoord;
		float4 pos : SV_Position;
	};

	VSOut main(float2 pos : Position2D, float2 tex : TexCoord){
		VSOut vso;

		vso.tex = tex;
		vso.tex.y = 1.0f - vso.tex.y; //In hlsl we slip the tex coords here so render targets behave the same across APIs
		vso.pos = mul(modelProjection, float4(pos, 0.0f, 1.0f));

		return vso;
	}
		)";
		std::string source_ps = R"(
	Texture2D albedoTexture : register(t1);
	SamplerState albedoSampler : register(s1);

	cbuffer colourBufferData : register(b12){
		float4 colour;
	};

	float4 main(float2 texCoord : TexCoord) : SV_TARGET{
		return albedoTexture.Sample(albedoSampler, texCoord) * colour;
	}
		)";
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
					//NSString* librarySource = [NSString stringWithCString:shader_2D.c_str() encoding:[NSString defaultCStringEncoding]];
					//library = [mtlDevice newLibraryWithSource:librarySource options:nil error:&error];
					
					std::string vs_string = ShaderTranspiler::compile(source_vs, ShaderType::Vertex, ShaderOutputType::MSL);
					std::string ps_string = ShaderTranspiler::compile(source_ps, ShaderType::Pixel, ShaderOutputType::MSL);
					
					NSString* vs = [NSString stringWithCString:vs_string.c_str() encoding:[NSString defaultCStringEncoding]];
					NSString* ps = [NSString stringWithCString:ps_string.c_str() encoding:[NSString defaultCStringEncoding]];
					
					id<MTLLibrary> vs_lib = [mtlDevice newLibraryWithSource:vs options:nil error:&error];
					if(error.code != 0){
						for (NSString* key in [error userInfo]) {
							NSString* value = [error userInfo][key];
							CLV_LOG_ERROR("Error in function '{0}': {1}", CLV_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
						}
					}
					
					id<MTLLibrary> ps_lib = [mtlDevice newLibraryWithSource:ps options:nil error:&error];
					if(error.code != 0){
						for (NSString* key in [error userInfo]) {
							NSString* value = [error userInfo][key];
							CLV_LOG_ERROR("Error in function '{0}': {1}", CLV_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
						}
					}
					
					vertexShader = [vs_lib newFunctionWithName:@"main0"];
					pixelShader = [ps_lib newFunctionWithName:@"main0"];
					
					return;
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
