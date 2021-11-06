#include "Membrane/ReflectionHelper.hpp"

#include <Clove/Components/TransformComponent.hpp>
#include <Clove/Reflection/Reflection.hpp>
#include <Clove/ReflectionAttributes.hpp>
#include <msclr/marshal_cppstd.h>

using namespace clove;

namespace membrane {
    namespace {
        System::Collections::Generic::List<EditorTypeInfo ^> ^ constructMembers(std::vector<reflection::MemberInfo> const &members) {
            auto editorVisibleMemers{ gcnew System::Collections::Generic::List<EditorTypeInfo ^>{} };

            for(auto const &member : members) {
                if(std::optional<EditorEditableMember> attribute{ member.attributes.get<EditorEditableMember>() }) {
                    auto memberInfo{ gcnew EditorTypeInfo{} };
                    memberInfo->typeName    = gcnew System::String{ member.name.c_str() };
                    memberInfo->displayName = gcnew System::String{ attribute->name.value_or(member.name).c_str() };
                    if(reflection::TypeInfo const *memberTypeInfo{ reflection::getTypeInfo(member.id) }) {
                        memberInfo->members = constructMembers(memberTypeInfo->members);
                    } else {
                        memberInfo->members = gcnew System::Collections::Generic::List<EditorTypeInfo ^>{};
                    }

                    editorVisibleMemers->Add(memberInfo);
                }
            }

            return editorVisibleMemers;
        }

        EditorTypeInfo ^ constructEditorTypeInfo(reflection::TypeInfo const *typeInfo) {
            EditorVisibleComponent const visibleAttribute{ typeInfo->attributes.get<EditorVisibleComponent>().value() };

            auto editorTypeInfo{ gcnew EditorTypeInfo{} };
            editorTypeInfo->typeName    = gcnew System::String{ typeInfo->name.c_str() };
            editorTypeInfo->displayName = gcnew System::String{ visibleAttribute.name.value_or(typeInfo->name).c_str() };
            editorTypeInfo->members     = constructMembers(typeInfo->members);

            return editorTypeInfo;
        }
    }

    System::Collections::Generic::List<EditorTypeInfo ^> ^ ReflectionHelper::getAvailableTypes() {
        System::Collections::Generic::List<EditorTypeInfo ^> ^ list { gcnew System::Collections::Generic::List<EditorTypeInfo ^>{} };

        std::vector<reflection::TypeInfo const *> editorTypes{ reflection::getTypesWithAttribute<EditorVisibleComponent>() };
        for(auto const *typeInfo : editorTypes) {
            list->Add(constructEditorTypeInfo(typeInfo));
        }

        return list;
    }

    EditorTypeInfo ^ ReflectionHelper::getInfoForType(System::String ^ typeName) {
        System::String ^ managedName { typeName };
        return constructEditorTypeInfo(reflection::getTypeInfo(msclr::interop::marshal_as<std::string>(managedName)));
    }
}