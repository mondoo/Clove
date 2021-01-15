#Reflection
add_executable(ReflectionTest ReflectionTests.cpp)
target_link_libraries(ReflectionTest PRIVATE CONAN_PKG::gtest CloveReflection)
add_test(NAME ReflectionTest COMMAND ReflectionTest)