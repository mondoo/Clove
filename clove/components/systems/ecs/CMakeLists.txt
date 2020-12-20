set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/ECS)

#Library
add_library(
    CloveECS STATIC

	${INCLUDE}/ComponentManager.hpp
	${INCLUDE}/ComponentManager.inl
	${SOURCE}/ComponentManager.cpp
	${INCLUDE}/ECSEvents.hpp
	${INCLUDE}/Entity.hpp
	${INCLUDE}/EntityManager.hpp
	${INCLUDE}/EntityManager.inl
	${SOURCE}/EntityManager.cpp
)

#Includes
target_include_directories(
    CloveECS
    
	PUBLIC
		include

)

#Libraries
target_link_libraries(
    CloveECS

	PUBLIC
		CloveDefinitions
		CloveDeltaTime
		CloveEvent
		CloveMemory
		CloveTimer
)

if(GAR_BUILD_TESTS)
	add_subdirectory(tests)
endif()