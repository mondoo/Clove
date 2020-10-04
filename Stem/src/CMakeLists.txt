set(STM_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(STM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Stem)

add_library(Stem STATIC)

target_sources(
	Stem
	PRIVATE
		${STM_INCLUDE}/EntryPoint.hpp

		${STM_INCLUDE}/Application.hpp
		${STM_SOURCE}/Application.cpp
)