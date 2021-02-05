#Serialisation
add_executable(YamlSerialisationTest YamlSerialisationTests.cpp)
target_link_libraries(YamlSerialisationTest PRIVATE CONAN_PKG::gtest CloveSerialisation)
add_test(NAME YamlSerialisationTest COMMAND YamlSerialisationTest)

#Deerialisation
add_executable(YamlDeserialisationTest YamlDeserialisationTests.cpp)
target_link_libraries(YamlDeserialisationTest PRIVATE CONAN_PKG::gtest CloveSerialisation)
add_test(NAME YamlDeserialisationTest COMMAND YamlDeserialisationTest)

add_custom_command(TARGET YamlDeserialisationTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/files" "$<TARGET_FILE_DIR:YamlDeserialisationTest>/files"
)