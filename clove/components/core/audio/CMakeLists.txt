set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Audio)

add_subdirectory(external)

#Library
add_library(
    CloveAudio STATIC

    ${INCLUDE}/Audio.hpp
	${SOURCE}/Audio.cpp
	${INCLUDE}/AudioBuffer.hpp
	${INCLUDE}/AudioDevice.hpp
	${INCLUDE}/AudioFactory.hpp
	${INCLUDE}/AudioListener.hpp
    ${INCLUDE}/AudioSource.hpp
        
    ${INCLUDE}/OpenAL/ALBuffer.hpp
	${SOURCE}/OpenAL/ALBuffer.cpp
	${INCLUDE}/OpenAL/ALDevice.hpp
	${SOURCE}/OpenAL/ALDevice.cpp
	${INCLUDE}/OpenAL/ALError.hpp
	${SOURCE}/OpenAL/ALError.cpp
	${INCLUDE}/OpenAL/ALFactory.hpp
	${SOURCE}/OpenAL/ALFactory.cpp
	${INCLUDE}/OpenAL/ALListener.hpp
	${SOURCE}/OpenAL/ALListener.cpp
	${INCLUDE}/OpenAL/ALSource.hpp
	${SOURCE}/OpenAL/ALSource.cpp
)

#Includes
target_include_directories(
    CloveAudio
    
	PUBLIC
		include

)

#Libraries
target_link_libraries(
    CloveAudio

    PUBLIC
        CloveCast
        CloveLog
        CloveMaths

    PRIVATE
        OpenAL
)