#include "Clove/Graphics/ShaderCompiler.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <file_includer.h>
#include <fstream>
#include <libshaderc_util/file_finder.h>
#include <shaderc/shaderc.hpp>
#include <spirv.hpp>
#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>
#include <spirv_msl.hpp>
#include <sstream>

namespace garlic::clove::ShaderCompiler {
    namespace {
        shaderc_shader_kind getShadercStage(Shader::Stage stage) {
            switch(stage) {
                case Shader::Stage::Vertex:
                    return shaderc_vertex_shader;
                case Shader::Stage::Pixel:
                    return shaderc_fragment_shader;
                default:
                    CLOVE_ASSERT("Unsupported shader stage {0}", CLOVE_FUNCTION_NAME);
                    return shaderc_vertex_shader;
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

        std::vector<std::byte> spirvToHLSL(std::vector<uint32_t> const &sprivSource) {
            CLOVE_ASSERT("SPIR-V to HLSL not supported!");
            return {};
        }

        std::vector<std::byte> spirvToMSL(Shader::Stage shaderStage, std::vector<uint32_t> const &spirvSource) {
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
            if(shaderStage == Shader::Stage::Vertex) {
                for(auto &resource : resources.stage_inputs) {
                    uint32_t const location{ msl.get_decoration(resource.id, spv::DecorationLocation) };
                    std::string str{ msl.get_decoration_string(resource.id, spv::DecorationUserSemantic) };

                    msl.set_name(resource.id, str);
                }
            }

            //return msl.compile();

            CLOVE_ASSERT("SPIR-V to HLSL not fully supported!");
            return {};
        }
    }

    std::vector<std::byte> compileFromFile(std::string_view filePath, Shader::Stage shaderStage, ShaderType outputType) {
        auto const source{ readFile(filePath) };
        return compileFromSource({ std::begin(source), std::end(source) }, shaderStage, outputType);
    }

    std::vector<std::byte> compileFromSource(std::span<std::byte const> source, Shader::Stage shaderStage, ShaderType outputType) {
        shaderc_util::FileFinder fileFinder{};
        //TEMP: Hardcoding in a search path for testing
        fileFinder.search_path().push_back("/home/alex/Documents/Dev/sandbox/garlic/clove/source/Shaders");
        auto fileIncluder{ std::make_unique<glslc::FileIncluder>(&fileFinder) };

        shaderc::CompileOptions options{};
        options.SetIncluder(std::move(fileIncluder));

        shaderc::Compiler compiler{};

        auto spirv = compiler.CompileGlslToSpv(reinterpret_cast<char const *>(std::data(source)), std::size(source), getShadercStage(shaderStage), "\0", options);
        std::vector<uint32_t> spirvSource{ spirv.begin(), spirv.end() };

        switch(outputType) {
            default:
            case ShaderType::SPIRV: {
                std::vector<std::byte> bytes;
                bytes.resize(std::size(spirvSource) * sizeof(uint32_t));
                memcpy(std::data(bytes), std::data(spirvSource), std::size(bytes));
                return bytes;
            }
            case ShaderType::HLSL:
                return spirvToHLSL(spirvSource);
            case ShaderType::MSL:
                return spirvToMSL(shaderStage, spirvSource);
        }
    }
}
