#include "Membrane/ReflectionHelper.hpp"

#include <Clove/Components/TransformComponent.hpp>
#include <msclr/marshal_cppstd.h>
#include <Clove/Reflection/Reflection.hpp>
#include <Clove/ReflectionAttributes.hpp>

using namespace clove;

namespace membrane {
    namespace {
        EditorTypeInfo^ constructEditorTypeInfo(reflection::TypeInfo const* typeInfo){
            EditorVisible const visibleAttribute{ typeInfo->attributes.get<EditorVisible>().value() };

            auto editorTypeInfo{ gcnew EditorTypeInfo{} };
            editorTypeInfo->typeName    = gcnew System::String{ typeInfo->name.c_str() };
            editorTypeInfo->displayName = gcnew System::String{ visibleAttribute.name.value_or(typeInfo->name).c_str() };
            editorTypeInfo->members     = gcnew System::Collections::Generic::List<TypeMemberInfo ^>{};

            for(auto const &member : typeInfo->members) {
                if(std::optional<EditorVisible> attribute{ member.attributes.get<EditorVisible>() }) {
                    auto memberInfo{ gcnew TypeMemberInfo{} };
                    memberInfo->typeName    = gcnew System::String{ member.name.c_str() };
                    memberInfo->displayName = gcnew System::String{ attribute->name.value_or(member.name).c_str() };

                    editorTypeInfo->members->Add(memberInfo);
                }
            }

            return editorTypeInfo;
        }
    }

    System::Collections::Generic::List<EditorTypeInfo ^> ^ ReflectionHelper::getAvailableTypes() {
        System::Collections::Generic::List<EditorTypeInfo ^> ^ list { gcnew System::Collections::Generic::List<EditorTypeInfo ^>{} };

        std::vector<reflection::TypeInfo const *> editorTypes{ reflection::getTypesWithAttribute<EditorVisible>() };
        for(auto const *typeInfo : editorTypes) {
            list->Add(constructEditorTypeInfo(typeInfo));
        }

        return list;
    }

    EditorTypeInfo ^ ReflectionHelper::getInfoForType(System::String ^ typeName) {
        System::String ^ managedName{ typeName };
        return constructEditorTypeInfo(reflection::getTypeInfo(msclr::interop::marshal_as<std::string>(managedName)));
    }
}