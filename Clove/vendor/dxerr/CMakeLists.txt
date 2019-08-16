project(dxerr)

add_library(
    dxerr
    STATIC
    dxerr.h
    dxerr.cpp
    DXGetErrorDescription.inl
    DXGetErrorString.inl
    DXTrace.inl
)

target_include_directories(
	dxerr
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)