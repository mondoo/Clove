#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
template<typename T>
	void Material::setData(BufferBindingPoint bindingPoint, T&& data, gfx::ShaderType shaderType){
		if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
			if(auto sbo = std::dynamic_pointer_cast<ShaderBufferObject<T>>(iter->second)){
				sbo->update(std::forward<T>(data));
				return;
			}
		}
		shaderData[bindingPoint] = BindableFactory::createShaderBufferObject<T>(shaderType, bindingPoint, std::forward<T>(data));
	}
}