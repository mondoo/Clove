--GLOBALS
outputdir = "%{cfg.platform}/%{cfg.buildcfg}"

targetdir_clv = "Built/bin/"..outputdir.."/%{prj.name}"
objdir_clv = "Built/intermediate/"..outputdir.."/%{prj.name}"

targetdir_vendor = "Built/bin/"..outputdir.."/vendor/%{prj.name}"
objdir_vendor = "Built/intermediate/"..outputdir.."/vendor/%{prj.name}"

hlslout_dir = "../Sandbox/"
hlslout_com = hlslout_dir.."%{file.basename}"..".cso"

--Workspace Settings
workspace "Clove"
	startproject "Sandbox"

	systemversion "latest"
	cppdialect "C++17"

	configurations{
		"Debug",
		"Development",
		"Profiling",
		"Release"
	}

	platforms{
		"Win64"
	}

	defines{
		"_ENABLE_EXTENDED_ALIGNED_STORAGE"
	}

	--Platform filters
	filter "platforms:Win64"
		system "Windows"
		architecture "x64"
		staticruntime "On"

	--System filters
	filter "system:Windows"
		defines{
			"CLV_PLATFORM_WINDOWS=1"
		}

	--Configuration filters
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

		defines {
			"CLV_DEBUG=1"
		}
		
	filter "configurations:Development"
		runtime "Debug"
		symbols "On"
		optimize "Debug"

		defines {
			"CLV_DEVELOPMENT=1"
		}
		
	filter "configurations:Profiling"
		runtime "Release"
		symbols "Off"
		optimize "On"

		defines {
			"CLV_PROFILING=1"
		}

	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "Full"

		defines {
			"CLV_RELEASE=1"
		}

--Clove Dependencies
group "Dependencies"
--IMGUI
project "ImGui"
	location "Clove/vendor/imgui"
	kind "StaticLib"
	language "C++"
		
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
        "%{prj.location}/**.h",
        "%{prj.location}/**.cpp",
    }
	
	excludes{
		"%{prj.location}/examples/**.h",
		"%{prj.location}/examples/**.cpp",
		"%{prj.location}/misc/**.h",
		"%{prj.location}/misc/**.cpp",
	}
	
	defines{
		"IMGUI_USER_CONFIG=\"../../src/Clove/ImGui/ImGuiConfig.hpp\"",
		"IMGUI_DISABLE_INCLUDE_IMCONFIG_H",
		"_CRT_SECURE_NO_WARNINGS"
	}    

--GLAD
project "Glad"
	location "Clove/vendor/Glad"
    kind "StaticLib"
    language "C"
    
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
        "%{prj.location}/include/**.h",
        "%{prj.location}/include/**.h",
        "%{prj.location}/src/**.c"
    }

	includedirs{
		"%{prj.location}/include"
	}

--dxerr
project "dxerr"
	location "Clove/vendor/dxerr"
	kind "StaticLib"
	language "C++"

	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
		"%{prj.location}/**.h",
		"%{prj.location}/**.inl",
		"%{prj.location}/**.cpp",
	}
	
--stb
project "stb"
	location "Clove/vendor/stb"
	kind "StaticLib"
	language "C++"
	
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)
	
	files{
		"%{prj.location}/**.h",
		"%{prj.location}/**.cpp",
	}

--End: Dependencies

--CLOVE
-- Inlcude direction relative to the roof folder (solution directory)
includeDir = {}
includeDir["Glad"]	= "Clove/vendor/Glad/include"
includeDir["ImGui"] = "Clove/vendor/imgui"
includeDir["glm"]	= "Clove/vendor/glm"
includeDir["stb"]	= "Clove/vendor/stb"
includeDir["dxerr"]	= "Clove/vendor/dxerr"

group ""
project "Clove"
	location "Clove"
	language "C++"
	kind "StaticLib"

	targetdir(targetdir_clv)
	objdir(objdir_clv)

	pchheader "clvpch.hpp"
	pchsource "Clove/src/clvpch.cpp"

	files{
		--src
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.cpp",

		--shader
		"%{prj.name}/src/**.glsl",
		"%{prj.name}/src/**.hlsl",
	}

	includedirs{
		--Clove
		"%{prj.name}/src",
		
		--Libs
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb}",
		"%{includeDir.dxerr}",

		--Misc vendor
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/OBJ-Loader/source",
		"%{prj.name}/vendor/Event-Dispatcher",
		"%{prj.name}/vendor/wglext",
	}

	links{
		"Glad",
		"ImGui",
		"dxerr",
		"stb",
		
		"opengl32.lib"
	}

	defines{
		"CLV_ENGINE=1",
		"GLFW_INCLUDE_NONE"
	}

	postbuildcommands{
		("{COPY} "..hlslout_dir.."**.cso \"../Built/bin/"..outputdir.."/Sandbox/\"")
	}

	--hlsl shaders
	filter "files:**.hlsl"
		shadermodel "5.0"
		shaderobjectfileoutput(hlslout_com)
	filter "files:**-vs.hlsl"
		shadertype "Vertex"
	filter "files:**-ps.hlsl"
		shadertype "Pixel"

--Sandbox
group ""
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir(targetdir_clv)
	objdir(objdir_clv)

	files{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"Clove/vendor/spdlog/include",

		"%{includeDir.ImGui}",
		"%{includeDir.glm}",

		"Clove/src",
		"Clove/vendor",
	}

	links{
		"Clove",
	}