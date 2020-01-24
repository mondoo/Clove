#include "Clove/Graphics/Direct3D/D3DShader.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/ShaderHeaders.hpp"

#include <d3dcompiler.h>

namespace clv::gfx::d3d{
	D3DShader::D3DShader(ID3D11Device& d3dDevice, const ShaderDescriptor& descriptor, std::string_view pathToShader)
		: descriptor(descriptor){
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if CLV_DEBUG
		flags |= D3DCOMPILE_DEBUG;
	#endif

		const D3D_SHADER_MACRO defines[] = {
			nullptr, nullptr
		};

		const char* charArray = pathToShader.data();
		std::size_t stringLength = mbsrtowcs(nullptr, &charArray, 0, nullptr);
		wchar_t* wideCharArray = new wchar_t[stringLength + 1]();
		stringLength = mbsrtowcs(wideCharArray, &charArray, stringLength + 1, nullptr);

		ID3DBlob* errorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile(wideCharArray, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", getShaderProfile(descriptor.stage), flags, 0, &shaderBlob, &errorBlob);
		if(FAILED(hr)){
			if(errorBlob){
				//TODO: Update to a clove log
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}
		}

		errorBlob->Release();
		delete[] wideCharArray;
	}

	D3DShader::D3DShader(D3DShader&& other) noexcept = default;

	D3DShader& D3DShader::operator=(D3DShader&& other) noexcept = default;

	D3DShader::~D3DShader() = default;

	const ShaderDescriptor& D3DShader::getDescriptor() const{
		return descriptor;
	}

	const Microsoft::WRL::ComPtr<ID3DBlob>& D3DShader::getShaderBlob() const{
		return shaderBlob;
	}

	LPCSTR D3DShader::getShaderProfile(const ShaderStage stage){
		switch(stage){
			case ShaderStage::Vertex:
				return "vs_5_0";
			case ShaderStage::Geometry:
				return "gs_5_0";
			case ShaderStage::Pixel:
				return "ps_5_0";
			default:
				CLV_ASSERT(false, "Unhandled shader stage in {0}", CLV_FUNCTION_NAME);
				return "";
		}
	}
}