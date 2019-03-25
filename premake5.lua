--GLOBALS
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Clove"
	architecture "x64"
	startproject "Sandbox"

	configurations{
		"Debug",
		"Release",
		"Dist"
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
		"IMGUI_DISABLE_INCLUDE_IMCONFIG_H"
	}
	
	filter "system:windows"
	    systemversion "latest"
	    cppdialect "C++17"
	    staticruntime "On"
	    
	filter { "system:windows", "configurations:Release" }
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
    
	filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "10.0.17134.0"
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
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}

    filter { "system:windows", "configurations:Release" }
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
    
	filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "latest"
        staticruntime "On"
        
    filter { "system:windows", "configurations:Release" }
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

		"%{prj.name}/res/**.shader",
		"%{prj.name}/res/**.png",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb/**.cpp",
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb}"
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