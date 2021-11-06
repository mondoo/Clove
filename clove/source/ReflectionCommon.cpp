#include "Clove/ReflectionAttributes.hpp"

#include <Clove/Maths/Quaternion.hpp>
#include <Clove/Maths/Vector.hpp>
#include <Clove/Reflection/Reflection.hpp>

//Provides reflection information for common types

CLOVE_REFLECT_BEGIN(clove::vec3f)
CLOVE_REFLECT_PROPERTY(x, clove::EditorEditableMember{})
CLOVE_REFLECT_PROPERTY(y, clove::EditorEditableMember{})
CLOVE_REFLECT_PROPERTY(z, clove::EditorEditableMember{})
CLOVE_REFLECT_END(clove::vec3f)

CLOVE_REFLECT_BEGIN(clove::quatf)
CLOVE_REFLECT_PROPERTY(x, clove::EditorEditableMember{})
CLOVE_REFLECT_PROPERTY(y, clove::EditorEditableMember{})
CLOVE_REFLECT_PROPERTY(z, clove::EditorEditableMember{})
CLOVE_REFLECT_PROPERTY(w, clove::EditorEditableMember{})
CLOVE_REFLECT_END(clove::quatf)