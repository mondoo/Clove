#include "Clove/Components/StaticModelComponent.hpp"
#include "Clove/ReflectionAttributes.hpp"

#include <Clove/Reflection/Reflection.hpp>

CLOVE_REFLECT_BEGIN(clove::StaticModelComponent, clove::EditorVisibleComponent{.name = "Static Model Component"})
CLOVE_REFLECT_MEMBER(model, clove::EditorEditableMember{})
CLOVE_REFLECT_END(clove::StaticModelComponent)