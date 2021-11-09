#include <Clove/Log/Log.hpp>

namespace clove {
    template<typename BaseDescriptorSetType>
    void ValidationDescriptorSet<BaseDescriptorSetType>::write(GhaBuffer const &buffer, size_t const offset, size_t const range, DescriptorType const descriptorType, uint32_t const bindingSlot) {
        CLOVE_ASSERT_MSG(range > 0, "{0}: 'range' value cannot be 0.");

        BaseDescriptorSetType::write(buffer, offset, range, descriptorType, bindingSlot);
    }
}