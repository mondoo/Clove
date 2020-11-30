set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    BulbUI SHARED

    ${SOURCE}/ViewModel.cs
    ${SOURCE}/RelayCommand.cs   
)

#Target properties
set_target_properties(BulbUI PROPERTIES 
		VS_DOTNET_FRAMEWORK_VERSION "4.8"
        VS_DOTNET_REFERENCES "Microsoft.CSharp;PresentationCore;PresentationFramework;System;System.Core;System.Data;WindowsBase"
)