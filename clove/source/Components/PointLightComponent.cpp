#include "Clove/Components/PointLightComponent.hpp"

#include "Clove/ReflectionAttributes.hpp"

#include <Clove/Reflection/Reflection.hpp>

CLOVE_REFLECT_BEGIN(clove::PointLightComponent, clove::EditorVisibleComponent{ .name = "Point Light Component" })
CLOVE_REFLECT_PROPERTY(ambientColour, clove::EditorEditableMember{})
CLOVE_REFLECT_PROPERTY(diffuseColour, clove::EditorEditableMember{})
CLOVE_REFLECT_PROPERTY(specularColour, clove::EditorEditableMember{})
CLOVE_REFLECT_END(clove::PointLightComponent)