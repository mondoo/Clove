#include "Membrane/ReflectionHelper.hpp"

#include <Clove/Components/TransformComponent.hpp>
#include <msclr/marshal_cppstd.h>
#include <Clove/Reflection/Reflection.hpp>
#include <Clove/ReflectionAttributes.hpp>

using namespace clove;

namespace membrane {
    System::Collections::Generic::List<EditorTypeInfo ^> ^ ReflectionHelper::getAvailableTypes() {
        System::Collections::Generic::List<EditorTypeInfo ^> ^ list { gcnew System::Collections::Generic::List<EditorTypeInfo ^>{} };

        std::vector<reflection::TypeInfo const *> editorTypes{ reflection::getTypesWithAttribute<EditorVisible>() };
        for(auto const *typeInfo : editorTypes) {
            EditorVisible const visibleAttribute{ typeInfo->attributes.get<EditorVisible>().value() };

            auto editorTypeInfo{ gcnew EditorTypeInfo{} };
            editorTypeInfo->typeName    = gcnew System::String{ typeInfo->name.c_str() };
            editorTypeInfo->displayName = gcnew System::String{ visibleAttribute.name.value_or(typeInfo->name).c_str() };
            list->Add(editorTypeInfo);
        }

        return list;
    }
}