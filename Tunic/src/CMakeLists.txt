set(TNC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(TNC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Clove)

project(Tunic)

add_library(${PROJECT_NAME} STATIC)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		${TNC_SOURCE}/temp.cpp
)