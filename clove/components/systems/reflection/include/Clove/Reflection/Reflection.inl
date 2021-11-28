#include <type_traits>

namespace clove::reflection {
    template<typename Type>
    TypeInfo const *getTypeInfo() {
        return getTypeInfo(typeid(Type).hash_code());
    }
}