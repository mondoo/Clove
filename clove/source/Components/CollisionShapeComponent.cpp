#include "Clove/Components/CollisionShapeComponent.hpp"

#include "Clove/ReflectionAttributes.hpp"

#include <Clove/Reflection/Reflection.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/ECS/Entity.hpp>

CLOVE_REFLECT_BEGIN(clove::CollisionShapeComponent::Sphere)
CLOVE_REFLECT_MEMBER(radius, clove::EditorEditableMember{})
CLOVE_REFLECT_END

CLOVE_REFLECT_BEGIN(clove::CollisionShapeComponent::Cube)
CLOVE_REFLECT_MEMBER(halfExtents, clove::EditorEditableMember{})
CLOVE_REFLECT_END

CLOVE_REFLECT_BEGIN(clove::CollisionShapeComponent, clove::EditorVisibleComponent{
                                                        .name                    = "Collision Shape Component",
                                                        .onEditorCreateComponent = [](clove::Entity entity, clove::EntityManager &manager) -> uint8_t * {
                                                            return reinterpret_cast<uint8_t *>(&manager.addComponent<clove::CollisionShapeComponent>(entity));
                                                        },
                                                        .onEditorGetComponent = [](clove::Entity entity, clove::EntityManager &manager) -> uint8_t * {
                                                            return reinterpret_cast<uint8_t *>(&manager.getComponent<clove::CollisionShapeComponent>(entity));
                                                        },
                                                        .onEditorDestroyComponent = [](clove::Entity entity, clove::EntityManager &manager) { manager.removeComponent<clove::CollisionShapeComponent>(entity); },
                                                    })
CLOVE_REFLECT_MEMBER(shape, clove::EditorEditableDropdown{
                                .getDropdownMembers = []() -> std::vector<std::string> {
                                    return {
                                        "Sphere",
                                        "Cube"
                                    };
                                },
                                .setSelectedItem      = [](uint8_t *const memory, size_t offset, size_t size, std::string_view selection) {
                                    CLOVE_ASSERT(size == sizeof(clove::CollisionShapeComponent::ShapeVariant));
                                    auto *const shape{ reinterpret_cast<clove::CollisionShapeComponent::ShapeVariant *const>(memory) };

                                    if(selection == "Sphere") {
                                        *shape = clove::CollisionShapeComponent::Sphere{};
                                    } else if(selection == "Cube") {
                                        *shape = clove::CollisionShapeComponent::Cube{};
                                    } 
                                },
                                .getSelectedIndex     = [](uint8_t const *const memory, size_t offset, size_t size) -> size_t { 
                                    CLOVE_ASSERT(size == sizeof(clove::CollisionShapeComponent::ShapeVariant));
                                    auto *const shape{ reinterpret_cast<clove::CollisionShapeComponent::ShapeVariant const *const>(memory) };

                                    if(std::holds_alternative<clove::CollisionShapeComponent::Sphere>(*shape)) {
                                        return 0;
                                    } else if(std::holds_alternative<clove::CollisionShapeComponent::Cube>(*shape)) {
                                        return 1;
                                    } else {
                                        return -1;
                                    }
                                },
                                .getTypeInfoForMember = [](std::string_view member) -> reflection::TypeInfo const * { 
                                    if(member == "Sphere") {
                                        return clove::reflection::getTypeInfo<clove::CollisionShapeComponent::Sphere>();
                                    } else if(member == "Cube") {
                                        return clove::reflection::getTypeInfo<clove::CollisionShapeComponent::Cube>();
                                    } else {
                                        return nullptr;
                                    }
                                },
                            })
CLOVE_REFLECT_END