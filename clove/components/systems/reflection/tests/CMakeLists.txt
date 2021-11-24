#Reflection
add_executable(ReflectionTest ReflectionTests.cpp)
target_link_libraries(ReflectionTest PRIVATE CONAN_PKG::GTest CloveReflection)
add_test(NAME ReflectionTest COMMAND ReflectionTest)