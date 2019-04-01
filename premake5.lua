--GLOBALS
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Workspace Settings
workspace "Clove"
	startproject "Sandbox"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	platforms{
		"Win64-lib",
		"Win64-dll"
	}

	filter "platforms:Win64-lib"
		system "Windows"
		architecture "x64"

		defines{
			"CLV_STATIC=1",
			"CLV_DYNAMIC=0"
		}

	filter "platforms:Win64-dll"
		system "Windows"
		architecture "x64"

		defines{
			"CLV_STATIC=0",
			"CLV_DYNAMIC=1"
		}

	filter "system:Windows"
		systemversion "latest"
		cppdialect "C++17"

		defines{
			"CLV_PLATFORM_WINDOWS=1"
		}

	filter "configurations:Debug"
		defines {
			"CLV_DEBUG=1",
			"CLV_RELEASE=0",
			"CLV_DIST=0"
		}

	filter "configurations:Release"
		defines {
			"CLV_DEBUG=0",
			"CLV_RELEASE=1",
			"CLV_DIST=0",
		}

	filter "configurations:Dist"
		defines {
			"CLV_DEBUG=0",
			"CLV_RELEASE=0",
			"CLV_DIST=1"
		}

--Clove Dependencies
group "Dependencies"
--IMGUI
project "ImGui"
	location "Clove/vendor/imgui"
	kind "StaticLib"
	language "C++"
		
	targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.location}/intermediate/" .. outputdir .. "/%{prj.name}")

	files{
        "%{prj.location}/imconfig.h",
        "%{prj.location}/imgui.h",
        "%{prj.location}/imgui.cpp",
        "%{prj.location}/imgui_draw.cpp",
        "%{prj.location}/imgui_internal.h",
        "%{prj.location}/imgui_widgets.cpp",
        "%{prj.location}/imstb_rectpack.h",
        "%{prj.location}/imstb_textedit.h",
        "%{prj.location}/imstb_truetype.h",
        "%{prj.location}/imgui_demo.cpp"
    }

	defines{
		"IMGUI_USER_CONFIG=\"../../src/Clove/ImGui/ImGuiConfig.h\"",
		"IMGUI_DISABLE_INCLUDE_IMCONFIG_H",
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	filter "system:Windows"
	    staticruntime "On"
	    
	filter { "system:Windows", "configurations:Release or Dist" }
	    buildoptions "/MT"

--GFLW
project "GLFW"
	location "Clove/vendor/GLFW"
    kind "StaticLib"
    language "C"
    
	targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{prj.location}/intermediate/" .. outputdir .. "/%{prj.name}")

	files{
        "%{prj.location}/include/GLFW/glfw3.h",
        "%{prj.location}/include/GLFW/glfw3native.h",
        "%{prj.location}/src/glfw_config.h",
        "%{prj.location}/src/context.c",
        "%{prj.location}/src/init.c",
        "%{prj.location}/src/input.c",
        "%{prj.location}/src/monitor.c",
        "%{prj.location}/src/vulkan.c",
        "%{prj.location}/src/window.c"
    }

	defines{
		"_CRT_SECURE_NO_WARNINGS"
	}
    
	filter "system:Windows"
	    staticruntime "On"
        
        files{
            "%{prj.location}/src/win32_init.c",
            "%{prj.location}/src/win32_joystick.c",
            "%{prj.location}/src/win32_monitor.c",
            "%{prj.location}/src/win32_time.c",
            "%{prj.location}/src/win32_thread.c",
            "%{prj.location}/src/win32_window.c",
            "%{prj.location}/src/wgl_context.c",
            "%{prj.location}/src/egl_context.c",
            "%{prj.location}/src/osmesa_context.c"
        }

		defines{ 
            "_GLFW_WIN32"
		}

    filter { "system:Windows", "configurations:Release or Dist" }
        buildoptions "/MT"

--GLAD
project "Glad"
	location "Clove/vendor/Glad"
    kind "StaticLib"
    language "C"
    
	targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{prj.location}/intermediate/" .. outputdir .. "/%{prj.name}")

	files{
        "%{prj.location}/include/glad/glad.h",
        "%{prj.location}/include/KHR/khrplatform.h",
        "%{prj.location}/src/glad.c"
    }

	includedirs{
		"%{prj.location}/include"
	}
    
	filter "system:Windows"
	    staticruntime "On"
        
    filter { "system:Windows", "configurations:Release or Dist" }
        buildoptions "/MT"

--End: Dependencies
group ""

--CLOVE
-- Inlcude direction relative to the roof folder (solution directory)
includeDir = {}
includeDir["GLFW"]	= "Clove/vendor/GLFW/include"
includeDir["Glad"]	= "Clove/vendor/Glad/include"
includeDir["ImGui"] = "Clove/vendor/imgui"
includeDir["glm"]	= "Clove/vendor/glm"
includeDir["stb"]	= "Clove/vendor/stb"

project "Clove"
	location "Clove"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "clvpch.h"
	pchsource "Clove/src/clvpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/res/**.shader",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb/**.cpp",
		"%{prj.name}/vendor/OBJ-Loader/**.h"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",

		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb}",

		"%{prj.name}/vendor/OBJ-Loader/source"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	defines{
		"CLV_ENGINE=1",
		"CLV_EXPORT_DLL=1",
		"GLFW_INCLUDE_NONE"
	}

	filter "platforms:Win64-lib"
		kind "StaticLib"
		staticruntime "On"

	filter "platforms:Win64-dll"
		kind "SharedLib"
		staticruntime "Off"

	filter "kind:SharedLib"
		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "system:Windows"
		defines{
			"CLV_PLATFORM_WINDOWS=1"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release or Dist"
		runtime "Release"
		optimize "On"

--SANDBOX
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

		"%{includeDir.ImGui}",
		"%{includeDir.glm}",

		"Clove/src",
		"Clove/vendor"
	}

	links{
		"Clove",
	}

	filter "system:Windows"
		staticruntime "On"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release or Dist"
		optimize "On"

	filter {"platforms:Win64-lib", "configurations:Debug"}
		buildoptions "/MTd"

	filter {"platforms:Win64-lib", "configurations:Release or Dist"}
		buildoptions "/MT"

	filter {"platforms:Win64-dll", "configurations:Debug"}
		buildoptions "/MDd"

	filter {"platforms:Win64-dll", "configurations:Release or Dist"}
		buildoptions "/MD"