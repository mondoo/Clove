set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Audio)

#Library
add_library(
    CloveAudio STATIC

    ${INCLUDE}/Audio.hpp
	${SOURCE}/Audio.cpp
	${INCLUDE}/AhaBuffer.hpp
	${INCLUDE}/AhaDevice.hpp
	${INCLUDE}/AhaFactory.hpp
	${INCLUDE}/AhaListener.hpp
    ${INCLUDE}/AhaSource.hpp
        
    ${INCLUDE}/OpenAL/OpenAlBuffer.hpp
	${SOURCE}/OpenAL/OpenAlBuffer.cpp
	${INCLUDE}/OpenAL/OpenAlDevice.hpp
	${SOURCE}/OpenAL/OpenAlDevice.cpp
	${INCLUDE}/OpenAL/OpenAlError.hpp
	${SOURCE}/OpenAL/OpenAlError.cpp
	${INCLUDE}/OpenAL/OpenAlFactory.hpp
	${SOURCE}/OpenAL/OpenAlFactory.cpp
	${INCLUDE}/OpenAL/OpenAlListener.hpp
	${SOURCE}/OpenAL/OpenAlListener.cpp
	${INCLUDE}/OpenAL/OpenAlSource.hpp
	${SOURCE}/OpenAL/OpenAlSource.cpp
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
		CloveExpected
        CloveLog
        CloveMaths

    PRIVATE
        CONAN_PKG::openal
)