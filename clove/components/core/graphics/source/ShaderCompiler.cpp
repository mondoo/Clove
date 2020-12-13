#include "Clove/Graphics/ShaderCompiler.hpp"

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

namespace garlic::clove::ShaderCompiler {
    namespace {
        EShLanguage getEShStage(Shader::Stage stage) {
            switch(stage) {
                case Shader::Stage::Vertex:
                    return EShLanguage::EShLangVertex;
                case Shader::Stage::Pixel:
                    return EShLanguage::EShLangFragment;
                default:
                    CLOVE_ASSERT("Unsupported shader stage {0}", CLOVE_FUNCTION_NAME);
                    return EShLanguage::EShLangVertex;
            }
        }

        std::vector<std::byte> readFile(std::string_view filePath) {
            //Start at the end so we can get the file size
            std::basic_ifstream<std::byte> file(filePath.data(), std::ios::ate | std::ios::binary);

            if(!file.is_open()) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Failed to open file", CLOVE_FUNCTION_NAME);
                return {};
            }

            size_t const fileSize{ static_cast<size_t>(file.tellg()) };
            std::vector<std::byte> buffer(fileSize);

            file.seekg(0);
            file.read(buffer.data(), fileSize);

            file.close();

            return buffer;
        }

        std::vector<std::byte> spirvToHLSL(Shader::Stage stage, std::vector<uint32_t> const &sprivSource) {
            CLOVE_ASSERT("SPIR-V to HLSL not supported!");
            return {};
        }

        std::vector<std::byte> spirvToMSL(Shader::Stage stage, std::vector<uint32_t> const &spirvSource) {
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

            //return msl.compile();
            return {};
        }
    }

    std::vector<std::byte> compileFromFile(std::string_view filePath, ShaderType outputType) {
        //TODO: Use readFile
        return {};
    }

    std::vector<std::byte> compileFromSource(std::span<std::byte const> source, ShaderType outputType) {
        std::string shaderSource;
        //shaderSource.append(source);

        glslang::InitializeProcess();

        EShLanguage const eshstage{ EShLangFragment }; //TODO: Is stage required??
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
            return {};
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

        switch(outputType) {
            case ShaderType::SPIRV:
                CLOVE_ASSERT("SPIR-V output not supported!");
                return {};
            case ShaderType::HLSL:
                //return spirvToHLSL(stage, spirvSource);
                break;
            case ShaderType::MSL:
                //return spirvToMSL(stage, spirvSource);
                break;
        }

        return {};
    }
}
