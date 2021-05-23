add_executable(AssetPtrTest AssetPtrTests.cpp)
target_link_libraries(AssetPtrTest PRIVATE CONAN_PKG::gtest Clove)
add_test(NAME AssetPtrTest COMMAND AssetPtrTest)

target_compile_definitions(AssetPtrTest PRIVATE ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")