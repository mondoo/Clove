set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Reflection)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    CloveReflection STATIC
        ${INCLUDE}/Reflect.hpp
        ${INCLUDE}/Serialiser.hpp
        ${INCLUDE}/YamlSerialiser.hpp
        ${SOURCE}/YamlSerialiser.cpp
)

#Includes
target_include_directories(
    CloveReflection
    
	PUBLIC
		include
)

#Libraries
target_link_libraries(
    CloveReflection

	PUBLIC
        CloveDefinitions

    PRIVATE
        CONAN_PKG::yaml-cpp
)

if(GAR_BUILD_TESTS)
	add_subdirectory(tests)
endif()