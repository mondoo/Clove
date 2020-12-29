#include "ComponentContainer.hpp"

namespace garlic::clove{
    ComponentContainerInterface::ComponentContainerInterface() = default;

    ComponentContainerInterface::ComponentContainerInterface(ComponentContainerInterface &&other) noexcept = default;

    ComponentContainerInterface &ComponentContainerInterface::operator=(ComponentContainerInterface &&other) noexcept = default;

    ComponentContainerInterface::~ComponentContainerInterface() = default;
}