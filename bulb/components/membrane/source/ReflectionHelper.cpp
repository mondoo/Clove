#include "Membrane/ReflectionHelper.hpp"

#include <Clove/Components/TransformComponent.hpp>
#include <msclr/marshal_cppstd.h>

namespace membrane {
    System::Collections::Generic::List<TypeInfo ^> ^ ReflectionHelper::getAvailableTypes() {
        //TEMP: Just manually making one for the transform comp - will need a way to get these programatically
        TypeInfo ^ transformTypeInfo { gcnew TypeInfo{} };
        transformTypeInfo->name = gcnew System::String{ clove::reflection::getAttribute<clove::EditorVisible>(clove::reflection::TypeInfo<clove::TransformComponent>{})->name.value().c_str() };

        System::Collections::Generic::List<TypeInfo ^> ^ list { gcnew System::Collections::Generic::List<TypeInfo ^>{} };

        list->Add(transformTypeInfo);

        return list;
    }
}