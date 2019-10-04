#Wrapper between the C# editor and C++ engine

project(CloveWrapper)

add_library(
	CloveWrapper SHARED
		Wrapper.hpp
		Wrapper.cpp
)

#set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6")
#set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES "System")
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/clr /EHa")

#string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC
		PRIVATE
			Clove
		
)

#TODO copy across dll