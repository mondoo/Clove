set(CLV_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(CLV_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Clove)

add_library(Clove STATIC)

#Files
set(
	AudioFiles
		${CLV_INCLUDE}/Audio/Audio.hpp
		${CLV_SOURCE}/Audio/Audio.cpp
		${CLV_INCLUDE}/Audio/AudioBuffer.hpp
		${CLV_INCLUDE}/Audio/AudioFactory.hpp
		${CLV_INCLUDE}/Audio/AudioListener.hpp
		${CLV_INCLUDE}/Audio/AudioSource.hpp	
)

set(
	OpenALAudioFiles
		${CLV_INCLUDE}/Audio/OpenAL/ALBuffer.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALBuffer.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALError.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALError.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALFactory.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALFactory.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALListener.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALListener.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALSource.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALSource.cpp
)

set(
	ExceptionFiles
		${CLV_INCLUDE}/Exception/CloveException.hpp
		${CLV_SOURCE}/Exception/CloveException.cpp
)

set(
	AllFiles
	${CLV_INCLUDE}/Clove.hpp
	
		${AudioFiles}
			${OpenALAudioFiles}
		${ExceptionFiles}
)

#Organise the source tree
file(GLOB_RECURSE header ${CLV_INCLUDE}/*.h ${CLV_INCLUDE}/*.hpp ${CLV_INCLUDE}/*.inl)
file(GLOB_RECURSE source *.cpp *.m *.mm)
source_group(TREE ${CLV_INCLUDE} FILES ${header})
source_group(TREE ${CLV_SOURCE} FILES ${source})

source_group(PCH FILES 
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/Clove.dir/cmake_pch.hxx 
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/Clove.dir/cmake_pch.cxx
)

#Set the target source files
target_sources(
	Clove
	PRIVATE
		${AllFiles}
)

#Don't compile platform specific files
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set_source_files_properties(${LinuxPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set_source_files_properties(${WindowsPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set_source_files_properties(${WindowsPlatformFiles} ${LinuxPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()