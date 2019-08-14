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