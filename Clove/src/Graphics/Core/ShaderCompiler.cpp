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
                shader.setEnvTargetHlslFunctionality1();
                shader.setHlslIoMapping(true);
                
           // }

            shader.setAutoMapBindings(true);
            shader.setAutoMapLocations(true);

            shader.setEntryPoint("main");
            //shader.setInvertY(true); //Might need this later
            shader.setEnvInput(glslang::EShSourceHlsl, stage, glslang::EShClientVulkan, 500);
            shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);
            shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1);
            shader.parse(&builtInResources, 500, true, messages);

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


        if(outputType == ShaderOutputType::GLSL){
            spirv_cross::CompilerGLSL glsl(spirvSource);
            spirv_cross::CompilerGLSL::Options scoptions;

            scoptions.version = 460;
            scoptions.es = false;

            glsl.set_common_options(scoptions);

            return glsl.compile();
        } else if(outputType == ShaderOutputType::MSL){
            spirv_cross::CompilerMSL msl(spirvSource);

            return msl.compile();
        }

        return "";
    }
}