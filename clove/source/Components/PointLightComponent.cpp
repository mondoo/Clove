#include "Clove/Components/PointLightComponent.hpp"

#include "Clove/ReflectionAttributes.hpp"

#include <Clove/Reflection/Reflection.hpp>

CLOVE_REFLECT_BEGIN(clove::PointLightComponent, clove::EditorVisibleComponent{
                                                    .name                    = "Point Light Component",
                                                    .onEditorCreateComponent = [](clove::Entity entity, clove::EntityManager &manager) -> uint8_t * {
                                                        return reinterpret_cast<uint8_t *>(&manager.addComponent<clove::PointLightComponent>(entity));
                                                    },
                                                    .onEditorGetComponent = [](clove::Entity entity, clove::EntityManager &manager) -> uint8_t * {
                                                        return reinterpret_cast<uint8_t *>(&manager.getComponent<clove::PointLightComponent>(entity));
                                                    },
                                                    .onEditorDestroyComponent = [](clove::Entity entity, clove::EntityManager &manager) { manager.removeComponent<clove::PointLightComponent>(entity); },
                                                })
CLOVE_REFLECT_MEMBER(ambientColour, clove::EditorEditableMember{})
CLOVE_REFLECT_MEMBER(diffuseColour, clove::EditorEditableMember{})
CLOVE_REFLECT_MEMBER(specularColour, clove::EditorEditableMember{})
CLOVE_REFLECT_END