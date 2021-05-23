add_executable(AssetPtrTest AssetPtrTests.cpp)
target_link_libraries(AssetPtrTest PRIVATE CONAN_PKG::gtest Clove)
add_test(NAME AssetPtrTest COMMAND AssetPtrTest)