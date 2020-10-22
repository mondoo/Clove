set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Root/Delegate)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    Root.Delegate STATIC
        ${INCLUDE}/DelegateHandle.hpp
	    ${SOURCE}/DelegateHandle.cpp
	    ${INCLUDE}/MultiCastDelegate.hpp
	    ${INCLUDE}/MultiCastDelegate.inl
	    ${INCLUDE}/SingleCastDelegate.hpp
	    ${INCLUDE}/SingleCastDelegate.inl
)

#Include
target_include_directories(
	Root.Delegate 
	
	PUBLIC
		include
)

if(GAR_BUILD_TESTS)
	add_subdirectory(tests)
endif()