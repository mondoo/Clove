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
includeDir["GLFW"] = "Clove/vendor/GLFW/include"
includeDir["Glad"] = "Clove/vendor/Glad/include"
includeDir["ImGui"] = "Clove/vendor/imgui"

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
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"CLV_PLATFORM_WINDOWS",
			"CLV_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "CLV_DEBUG=1"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CLV_RELEASE=1"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CLV_DIST=1"
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
		"Clove/src"
	}

	links{
		"Clove"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"CLV_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CLV_DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "CLV_RELEASE"
		optimize "On"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines "CLV_DIST"
		optimize "On"
		buildoptions "/MD"