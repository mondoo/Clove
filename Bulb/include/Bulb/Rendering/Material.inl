#include "Clove/Graphics/GraphicsFactory.hpp"

namespace blb::rnd {
    template<typename DataType>
    void Material::setData(clv::gfx::BufferBindingPoint bindingPoint, DataType&& data, clv::gfx::ShaderStage shaderType) {
        setData(bindingPoint, &data, sizeof(data), shaderType);
    }
}