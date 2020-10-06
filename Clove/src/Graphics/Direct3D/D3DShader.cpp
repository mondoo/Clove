#include "Clove/Graphics/Direct3D/D3DShader.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"

#include <Root/Log/Log.hpp>
#include <d3dcompiler.h>

namespace clv::gfx::d3d {
    static UINT getShaderFlags() {
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if GARLIC_DEBUG
        flags |= D3DCOMPILE_DEBUG;
#endif

        return flags;
    }

    static LPCSTR getShaderProfile(const ShaderStage stage) {
        switch(stage) {
            case ShaderStage::Vertex:
                return "vs_5_0";
            case ShaderStage::Geometry:
                return "gs_5_0";
            case ShaderStage::Pixel:
                return "ps_5_0";
            default:
                GARLIC_ASSERT(false, "Unhandled shader stage in {0}", GARLIC_FUNCTION_NAME);
                return "";
        }
    }

    D3DShader::D3DShader(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice, ShaderDescriptor descriptor, std::string_view pathToShader)
        : factory(std::move(factory))
        , descriptor(std::move(descriptor)) {
        const D3D_SHADER_MACRO defines[] = {
            nullptr,
            nullptr
        };

        const char* charArray    = pathToShader.data();
        std::size_t stringLength = mbsrtowcs(nullptr, &charArray, 0, nullptr);
        wchar_t* wideCharArray   = new wchar_t[stringLength + 1]();
        stringLength             = mbsrtowcs(wideCharArray, &charArray, stringLength + 1, nullptr);

        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
        HRESULT hr                                 = D3DCompileFromFile(wideCharArray, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", getShaderProfile(this->descriptor.stage), getShaderFlags(), 0, &shaderBlob, &errorBlob);
        if(FAILED(hr)) {
            if(errorBlob != nullptr) {
                //TODO: Update to a clove log
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            }
        }

        delete[] wideCharArray;
    }

    D3DShader::D3DShader(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice, ShaderDescriptor descriptor, std::span<const std::byte> sourceBytes)
        : factory(std::move(factory))
        , descriptor(std::move(descriptor)) {
        const D3D_SHADER_MACRO defines[] = {
            nullptr,
            nullptr
        };

        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
        HRESULT hr                                 = D3DCompile(sourceBytes.data(), sourceBytes.size_bytes(), nullptr, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", getShaderProfile(this->descriptor.stage), getShaderFlags(), 0, &shaderBlob, &errorBlob);
        if(FAILED(hr)) {
            if(errorBlob != nullptr) {
                //TODO: Update to a clove log
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            }
        }
    }

    D3DShader::D3DShader(D3DShader&& other) noexcept = default;

    D3DShader& D3DShader::operator=(D3DShader&& other) noexcept = default;

    D3DShader::~D3DShader() = default;

    const std::shared_ptr<GraphicsFactory>& D3DShader::getFactory() const {
        return factory;
    }

    const ShaderDescriptor& D3DShader::getDescriptor() const {
        return descriptor;
    }

    const Microsoft::WRL::ComPtr<ID3DBlob>& D3DShader::getShaderBlob() const {
        return shaderBlob;
    }
}