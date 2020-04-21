set(STM_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(STM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Stem)

project(Stem)

add_library(${PROJECT_NAME} STATIC)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		${STM_INCLUDE}/EntryPoint.hpp

		${STM_INCLUDE}/Application.hpp
		${STM_SOURCE}/Application.cpp
)