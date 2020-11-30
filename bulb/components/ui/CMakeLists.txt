set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    BulbUI SHARED

    ${SOURCE}/ViewModel.cs
    
)

#Target properties
set_target_properties(BulbUI PROPERTIES 
		DOTNET_TARGET_FRAMEWORK_VERISON "v4.8"
        VS_DOTNET_REFERENCES "Microsoft.CSharp;System;"
)