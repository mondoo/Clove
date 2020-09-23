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