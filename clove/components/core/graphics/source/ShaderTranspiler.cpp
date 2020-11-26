#include "Clove/Graphics/ShaderTranspiler.hpp"

#define ENABLE_HLSL

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/SPVRemapper.h>
#include <SPIRV/disassemble.h>
#include <SPIRV/doc.h>
#include <StandAlone/ResourceLimits.h>
#include <fstream>
#include <glslang/Public/ShaderLang.h>
#include <spirv.hpp>
#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>
#include <spirv_msl.hpp>
#include <sstream>

namespace garlic::clove::ShaderTranspiler {
    static EShLanguage getEShStage(Shader::Stage stage) {
        switch(stage) {
            case Shader::Stage::Vertex:
                return EShLanguage::EShLangVertex;
            case Shader::Stage::Pixel:
                return EShLanguage::EShLangFragment;
            // case garlic::clove::Shader::Stage::Geometry:
            // 	return EShLanguage::EShLangGeometry;
            default:
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Unsupported shader stage {0}", CLOVE_FUNCTION_NAME);
                return EShLanguage::EShLangVertex;
        }
    }

    static std::string spirvToGlsl(Shader::Stage stage, std::vector<uint32_t> const &spirvSource) {
        spirv_cross::CompilerGLSL glsl(spirvSource);
        spirv_cross::CompilerGLSL::Options scoptions;

        scoptions.version                  = 460;
        scoptions.es                       = false;
        scoptions.enable_420pack_extension = false;
        glsl.set_common_options(scoptions);

        glsl.build_combined_image_samplers();

        spirv_cross::ShaderResources resources = glsl.get_shader_resources();

        //Make sure the combined image/samplers keep the binding id
        std::map<spirv_cross::ID, uint32_t> samplerBindings;
        for(auto &resource : resources.separate_samplers) {
            samplerBindings[resource.id] = glsl.get_decoration(resource.id, spv::DecorationBinding);
        }
        for(auto &remap : glsl.get_combined_image_samplers()) {
            glsl.set_decoration(remap.combined_id, spv::DecorationBinding, samplerBindings[remap.sampler_id]);
        }

        //Remap names to semantics
        if(stage == Shader::Stage::Vertex) {
            for(auto &resource : resources.stage_inputs) {
                const uint32_t location = glsl.get_decoration(resource.id, spv::DecorationLocation);
                std::string str         = glsl.get_decoration_string(resource.id, spv::DecorationUserSemantic);

                glsl.set_name(resource.id, str);
            }
        }

        // if(stage == Shader::Stage::Geometry) {
        // 	const uint32_t invocations = glsl.get_execution_mode_argument(spv::ExecutionMode::ExecutionModeInvocations);
        // 	if(invocations == 196624) { //Unset invocations default to this, unset it
        // 		glsl.unset_execution_mode(spv::ExecutionMode::ExecutionModeInvocations);
        // 	}
        // }

        return glsl.compile();
    }

    static std::string spirvToMSL(Shader::Stage stage, std::vector<uint32_t> const &spirvSource) {
        spirv_cross::CompilerMSL msl(spirvSource);
        spirv_cross::CompilerMSL::Options scoptions;

        scoptions.platform = spirv_cross::CompilerMSL::Options::Platform::macOS;
        msl.set_msl_options(scoptions);

        spirv_cross::ShaderResources resources = msl.get_shader_resources();

        auto const remapMSLBindings = [&msl](uint32_t const binding, spirv_cross::ID const resourceID) {
            spirv_cross::MSLResourceBinding resourceBinding;
            resourceBinding.stage    = msl.get_execution_model();
            resourceBinding.desc_set = msl.get_decoration(resourceID, spv::DecorationDescriptorSet);
            resourceBinding.binding  = binding;
            //Can cause issue if not all are set
            resourceBinding.msl_buffer  = binding;
            resourceBinding.msl_texture = binding;
            resourceBinding.msl_sampler = binding;
            msl.add_msl_resource_binding(resourceBinding);
        };

        //Set up correct buffer bindings
        for(auto &resource : resources.uniform_buffers) {
            uint32_t const binding{ msl.get_decoration(resource.id, spv::DecorationBinding) };
            remapMSLBindings(binding, resource.id);
        }

        //Set up correct texture bindings
        for(auto &resource : resources.separate_images) {
            uint32_t const binding{ msl.get_decoration(resource.id, spv::DecorationBinding) };
            remapMSLBindings(binding, resource.id);
        }
        for(auto &resource : resources.separate_samplers) {
            uint32_t const binding{ msl.get_decoration(resource.id, spv::DecorationBinding) };
            remapMSLBindings(binding, resource.id);
        }

        //Remap names to semantics
        if(stage == Shader::Stage::Vertex) {
            for(auto &resource : resources.stage_inputs) {
                uint32_t const location{ msl.get_decoration(resource.id, spv::DecorationLocation) };
                std::string str{ msl.get_decoration_string(resource.id, spv::DecorationUserSemantic) };

                msl.set_name(resource.id, str);
            }
        }

        return msl.compile();
    }

    std::string transpileFromFile(std::string_view filePath, Shader::Stage stage, ShaderType outputType) {
        std::ifstream stream(filePath.data());

        std::string line;
        std::stringstream ss;
        while(getline(stream, line)) {
            ss << line << '\n';
        }

        return transpileFromSource(ss.str(), stage, outputType);
    }

    std::string transpileFromBytes(char const *bytes, std::size_t const size, Shader::Stage stage, ShaderType outputType) {
        return transpileFromSource({ bytes, size }, stage, outputType);
    }

    std::string transpileFromSource(std::string_view source, Shader::Stage stage, ShaderType outputType) {
        std::string shaderSource;
        if(outputType == ShaderType::GLSL) {
            shaderSource = "#define GLSL\n\n";
        }
        shaderSource.append(source);

        glslang::InitializeProcess();

        EShLanguage const eshstage = getEShStage(stage);
        glslang::TShader shader(eshstage);

        char const *rawSource = shaderSource.data();
        shader.setStrings(&rawSource, 1);

        TBuiltInResource builtInResources = glslang::DefaultTBuiltInResource;
        EShMessages messages              = static_cast<EShMessages>(
            EShMsgSpvRules |
            EShMsgReadHlsl | EShMsgHlslOffsets | EShMsgHlslLegalization |
            EShMsgKeepUncalled | EShMsgSuppressWarnings);

        shader.setEnvTargetHlslFunctionality1();
        shader.setAutoMapBindings(true);
        shader.setHlslIoMapping(true);
        shader.setAutoMapLocations(true);

        shader.setEntryPoint("main");
        shader.setEnvInput(glslang::EShSourceHlsl, eshstage, glslang::EShClientOpenGL, 450);
        shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
        shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);

        shader.parse(&builtInResources, 100, true, messages);

        char const *log = shader.getInfoLog();

        if(strlen(log) > 0) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Error compiling shader: {0}", log);
            return "";
        }

        glslang::SpvOptions spvOptions{};
        spvOptions.validate = false;
#if CLOVE_DEBUG
        spvOptions.generateDebugInfo = true;
        spvOptions.disableOptimizer  = true;
#else
        spvOptions.generateDebugInfo = false;
        spvOptions.disableOptimizer  = false;
#endif

        std::vector<uint32_t> spirvSource;
        spv::SpvBuildLogger logger;
        glslang::TIntermediate *inter = shader.getIntermediate();
        glslang::GlslangToSpv(*inter, spirvSource, &logger, &spvOptions);

        glslang::FinalizeProcess();

        if(outputType == ShaderType::GLSL) {
            return spirvToGlsl(stage, spirvSource);
        } else if(outputType == ShaderType::MSL) {
            return spirvToMSL(stage, spirvSource);
        }

        return "";
    }
}
