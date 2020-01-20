#include "Clove/Graphics/Core/ShaderCompiler.hpp"

//#include "Clove/Graphics/Direct3D/ShaderHeaders.hpp"

#define ENABLE_HLSL

#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/disassemble.h"
#include "glslang/Include/ResourceLimits.h"
#include "glslang/Public/ShaderLang.h"

#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/SPVRemapper.h"
#include "SPIRV/disassemble.h"
#include "SPIRV/doc.h"
#include "StandAlone/ResourceLimits.h"
#include "glslang/Public/ShaderLang.h"

#include "spirv.hpp"
#include "spirv_glsl.hpp"
#include "spirv_hlsl.hpp"
#include "spirv_msl.hpp"

//#include <cstdlib>

namespace clv::gfx::ShaderCompiler{
	EShLanguage getEShStage(ShaderType type){
		switch(type){
			case clv::gfx::ShaderType::Vertex:
				return EShLanguage::EShLangVertex;
			case clv::gfx::ShaderType::Pixel:
				return EShLanguage::EShLangFragment;
			case clv::gfx::ShaderType::Geometry:
				return EShLanguage::EShLangGeometry;
			default:
				break;
		}
	}

	std::string compile(const std::string& source, ShaderType type, ShaderOutputType outputType){
		std::vector<uint32_t> spirvSource;

		//if(inputFormat == ShaderFormat::GLSL || inputFormat == ShaderFormat::HLSL){
		const char* strs = source.c_str();

		glslang::InitializeProcess();

		EShLanguage stage = getEShStage(type);
		glslang::TShader shader(stage);
		shader.setStrings(&strs, 1);

		TBuiltInResource builtInResources = glslang::DefaultTBuiltInResource;
		EShMessages messages = static_cast<EShMessages>(
			EShMsgSpvRules |
			EShMsgReadHlsl | EShMsgHlslOffsets | EShMsgHlslLegalization |
			EShMsgKeepUncalled | EShMsgSuppressWarnings);

		//if(inputFormat == ShaderFormat::HLSL){

		// }

		shader.setEnvTargetHlslFunctionality1();
		shader.setAutoMapBindings(true);
		shader.setHlslIoMapping(true);
		shader.setAutoMapLocations(true);

		shader.setEntryPoint("main");
		//shader.setInvertY(true); //Might need this later (there's also an option on the compiler below)
		shader.setEnvInput(glslang::EShSourceHlsl, stage, glslang::EShClientOpenGL, 450);
		shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);

		shader.parse(&builtInResources, 100, true, messages);

		glslang::SpvOptions spvOptions;
		spvOptions.validate = false;
		spvOptions.disableOptimizer = true;
		spvOptions.optimizeSize = false;

		spv::SpvBuildLogger logger;

		const char* log = shader.getInfoLog();

		if(strlen(log) > 0){
			return log;
		}

		glslang::TIntermediate* inter = shader.getIntermediate();

		glslang::GlslangToSpv(*inter, spirvSource, &logger, &spvOptions);

		glslang::FinalizeProcess();
		// }

		/*printf("Discovered semantics:");
		for(int i = 0; i < inter.sem)*/

		if(outputType == ShaderOutputType::GLSL){
			spirv_cross::CompilerGLSL glsl(spirvSource);
			spirv_cross::CompilerGLSL::Options scoptions;

			spirv_cross::ShaderResources resources = glsl.get_shader_resources();

			

			scoptions.version = 460;
			scoptions.es = false;
			//scoptions.vulkan_semantics = true;
			scoptions.enable_420pack_extension = false;

			glsl.set_common_options(scoptions);
			glsl.build_combined_image_samplers();

			// Get all sampled images in the shader.
			unsigned binding = 0;
			for(auto& resource : resources.separate_samplers){
				unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
				/*unsigned*/ binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
				printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

				// Modify the decoration to prepare it for GLSL.
				//glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

				// Some arbitrary remapping if we want.
				//glsl.set_decoration(resource.id, spv::DecorationBinding, binding);

				glsl.set_name(resource.id, resource.name);

				//res = resource.id;
			}

			

			// Give the remapped combined samplers new names.
// Here you can also set up decorations if you want (binding = #N).
			for(auto& remap : glsl.get_combined_image_samplers()){
				//This properly sets the binding - TODO: Need a more robust way to do this
				glsl.set_decoration(remap.combined_id, spv::DecorationBinding, binding);
			}

			//Remap names to semantics
			if(type == ShaderType::Vertex){
				for(auto& resource : resources.stage_inputs){
					unsigned location = glsl.get_decoration(resource.id, spv::DecorationLocation);
					std::string str = glsl.get_decoration_string(resource.id, spv::DecorationUserSemantic);
					printf("Input %s (%u) at binding = %u\n (%s)", resource.name.c_str(), resource.id, location, str.c_str());

					glsl.set_name(resource.id, str);
				}
			}

			return glsl.compile();
		} else if(outputType == ShaderOutputType::MSL){
			spirv_cross::CompilerMSL msl(spirvSource);
			//spirv_cross::CompilerMSL::Options scoptions;

			return msl.compile();
		}

		return "";
	}
}