set(
	Files
		Engine.hpp
		Engine.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Files})

add_library(
	Scape.Membrane SHARED
		${Files}
)

target_link_libraries(
	Scape.Membrane
		PUBLIC
		PRIVATE
			Stem
)

set_target_properties(Scape.Membrane PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(Scape.Membrane PROPERTIES DOTNET_TARGET_FRAMEWORK "netcoreapp3.1")
#set_target_properties(Scape.Membrane PROPERTIES VS_DOTNET_REFERENCES "System")