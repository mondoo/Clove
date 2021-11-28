#include "Clove/Components/PointLightComponent.hpp"

#include "Clove/ReflectionAttributes.hpp"

#include <Clove/Reflection/Reflection.hpp>

CLOVE_REFLECT_BEGIN(clove::PointLightComponent, clove::EditorVisibleComponent{ .name = "Point Light Component" })
CLOVE_REFLECT_MEMBER(ambientColour, clove::EditorEditableMember{})
CLOVE_REFLECT_MEMBER(diffuseColour, clove::EditorEditableMember{})
CLOVE_REFLECT_MEMBER(specularColour, clove::EditorEditableMember{})
CLOVE_REFLECT_END