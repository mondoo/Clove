set(STM_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(STM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Stem)

set(ApplicationFolder "Garlic/Stem")

#Stem
add_library(Stem STATIC)
set_target_properties(Stem PROPERTIES FOLDER ${ApplicationFolder})

#Includes
target_include_directories(
	Stem
	PUBLIC
		include
)

#Libraries
target_link_libraries(
	Stem 
	PUBLIC 
		Bulb
)

#Source
target_sources(
	Stem
	PRIVATE
		${STM_INCLUDE}/EntryPoint.hpp

		${STM_INCLUDE}/Application.hpp
		${STM_SOURCE}/Application.cpp
)