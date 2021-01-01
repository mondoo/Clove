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
        /**
         * @brief Keeps a map for shader source strings to provide as included files.
         */
        class EmbeddedSourceIncluder : public shaderc::CompileOptions::IncluderInterface {
            //VARIABLES
        private:
            std::unordered_map<std::string, std::string> includeSources;

            //FUNCTIONS
        public:
            EmbeddedSourceIncluder() = delete;
            EmbeddedSourceIncluder(std::unordered_map<std::string, std::string> includeSources)
                : includeSources{ std::move(includeSources) } {
            }

            EmbeddedSourceIncluder(EmbeddedSourceIncluder const &other)     = default;
            EmbeddedSourceIncluder(EmbeddedSourceIncluder &&other) noexcept = default;

            EmbeddedSourceIncluder &operator=(EmbeddedSourceIncluder const &other) = default;
            EmbeddedSourceIncluder &operator=(EmbeddedSourceIncluder &&other) noexcept = default;

            ~EmbeddedSourceIncluder() = default;

            shaderc_include_result *GetInclude(const char *requested_source, shaderc_include_type type, const char *requesting_source, size_t include_depth) override {
                auto result{ new shaderc_include_result };

                if(includeSources.find(requested_source) != includeSources.end()) {
                    std::string &source{ includeSources[requested_source] };

                    result->source_name        = requested_source;
                    result->source_name_length = strlen(requested_source);
                    result->content            = source.c_str();
                    result->content_length     = source.length();
                } else {
                    CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Requested source {1} was not found.", CLOVE_FUNCTION_NAME_PRETTY, requested_source);

                    std::string error{ "Source not available in map" };

                    result->content        = error.c_str();
                    result->content_length = error.length();
                }

                return result;
            }

            void ReleaseInclude(shaderc_include_result *data) override {
                delete data;
            }
        };

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

        std::vector<char> readFile(std::filesystem::path const &filePath) {
            //Start at the end so we can get the file size
            std::ifstream file(filePath.c_str(), std::ios::ate | std::ios::binary);

            if(!file.is_open()) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Failed to open file", CLOVE_FUNCTION_NAME);
                return {};
            }

            size_t const fileSize{ static_cast<size_t>(file.tellg()) };
            std::vector<char> buffer(fileSize);

            file.seekg(0);
            file.read(buffer.data(), fileSize);

            file.close();

            return buffer;
        }

        std::vector<uint32_t> spirvToHLSL(std::vector<uint32_t> const &sprivSource) {
            CLOVE_ASSERT("SPIR-V to HLSL not supported!");
            return {};
        }

        std::vector<uint32_t> spirvToMSL(Shader::Stage shaderStage, std::vector<uint32_t> const &spirvSource) {
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

        Expected<std::vector<uint32_t>, std::runtime_error> compile(std::string_view source, std::unique_ptr<shaderc::CompileOptions::IncluderInterface> includer, std::string_view shaderName, Shader::Stage shaderStage, ShaderType outputType) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Compiling shader {0}...", shaderName);

            shaderc::CompileOptions options{};
            options.SetIncluder(std::move(includer));
#if !CLOVE_DEBUG
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
#else
            options.SetOptimizationLevel(shaderc_optimization_level_zero);
#endif

            shaderc::Compiler compiler{};
            shaderc::SpvCompilationResult spirvResult{ compiler.CompileGlslToSpv(source.data(), source.size(), getShadercStage(shaderStage), shaderName.data(), options) };
            
            if(spirvResult.GetCompilationStatus() != shaderc_compilation_status_success) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to compile shader {0}:\n\t{1}", shaderName, spirvResult.GetErrorMessage());
                return Unexpected{ std::runtime_error{ "Failed to compile shader. See output log for details." } };
            }

            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Debug, "Successfully compiled shader {0}!", shaderName);

            std::vector<uint32_t> spirvSource{ spirvResult.begin(), spirvResult.end() };

            switch(outputType) {
                default:
                case ShaderType::SPIRV:
                    return spirvSource;
                case ShaderType::HLSL:
                    return spirvToHLSL(spirvSource);
                case ShaderType::MSL:
                    return spirvToMSL(shaderStage, spirvSource);
            }
        }
    }

    Expected<std::vector<uint32_t>, std::runtime_error> compileFromFile(std::filesystem::path const &file, Shader::Stage shaderStage, ShaderType outputType) {
        if(!file.has_filename()) {
            return Unexpected{ std::runtime_error{ "Path does not have a file name" } };
        }

        shaderc_util::FileFinder fileFinder{};
        fileFinder.search_path().push_back(std::filesystem::path{ file }.remove_filename().c_str());

        std::vector<char> source{ readFile(file) };
        auto fileIncluder{ std::make_unique<glslc::FileIncluder>(&fileFinder) };
        std::string shaderName{ file.stem() };

        return compile({ source.data(), source.size() }, std::move(fileIncluder), shaderName, shaderStage, outputType);
    }

    Expected<std::vector<uint32_t>, std::runtime_error> compileFromSource(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, Shader::Stage shaderStage, ShaderType outputType) {
        return compile(source, std::make_unique<EmbeddedSourceIncluder>(std::move(includeSources)), shaderName, shaderStage, outputType);
    }
}
