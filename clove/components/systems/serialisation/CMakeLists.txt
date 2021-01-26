set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Serialisation)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    CloveSerialisation STATIC
        ${INCLUDE}/Serialiser.hpp
        ${INCLUDE}/Serialiser.inl
        ${SOURCE}/Serialiser.cpp
        ${INCLUDE}/YamlSerialiser.hpp
        ${SOURCE}/YamlSerialiser.cpp
)

#Includes
target_include_directories(
    CloveSerialisation
    
	PUBLIC
		include
)

#Libraries
target_link_libraries(
    CloveSerialisation

	PUBLIC
        CloveDefinitions

    PRIVATE
        CONAN_PKG::yaml-cpp
)

if(GAR_BUILD_TESTS)
	add_subdirectory(tests)
endif()