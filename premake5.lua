workspace "Clove"
	architecture "x64"
	startproject "Sandbox"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Inlcude direction relative to the roof folder (solution directory)
includeDir = {}
includeDir["GLFW"]	= "Clove/vendor/GLFW/include"
includeDir["Glad"]	= "Clove/vendor/Glad/include"
includeDir["ImGui"] = "Clove/vendor/imgui"
includeDir["glm"]	= "Clove/vendor/glm"

group "Dependencies"
	include "Clove/vendor/GLFW"
	include "Clove/vendor/Glad"
	include "Clove/vendor/imgui"
group ""

project "Clove"
	location "Clove"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "clvpch.h"
	pchsource "Clove/src/clvpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	defines{
		"ENGINE=1",
		"CLV_BUILD_DLL=1",
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"CLV_PLATFORM_WINDOWS"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines {
			"CLV_DEBUG=1",
			"CLV_RELEASE=0",
			"CLV_DIST=0"
		}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines {
			"CLV_DEBUG=0",
			"CLV_RELEASE=1",
			"CLV_DIST=0"
		}
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines {
			"CLV_DEBUG=0",
			"CLV_RELEASE=0",
			"CLV_DIST=1"
		}
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("intermediate/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"Clove/vendor/spdlog/include",
		"%{includeDir.glm}",
		"Clove/src",
		"Clove/vendor"
	}

	links{
		"Clove"
	}

	defines{
		"ENGINE=0",
		"CLV_BUILD_DLL=0"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"CLV_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines {
			"CLV_DEBUG=1",
			"CLV_RELEASE=0",
			"CLV_DIST=0"
		}
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines {
			"CLV_DEBUG=0",
			"CLV_RELEASE=1",
			"CLV_DIST=0"
		}
		optimize "On"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines {
			"CLV_DEBUG=0",
			"CLV_RELEASE=0",
			"CLV_DIST=1"
		}
		optimize "On"
		buildoptions "/MD"