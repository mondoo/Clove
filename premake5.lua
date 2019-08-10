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
		"Win32",
		"Win64",
		"Linux",
	}

	defines{
		"_ENABLE_EXTENDED_ALIGNED_STORAGE"
	}

	--Platform filters
	filter "platforms:Win32"
		system "windows"
		architecture "x86"
		staticruntime "on"

	filter "platforms:Win64"
		system "windows"
		architecture "x86_64"
		staticruntime "on"

	filter "platforms:Linux"
		system "linux"
		architecture "x86_64"
		staticruntime "on"

	--System filters
	filter "system:windows"
		defines{
			"CLV_PLATFORM_WINDOWS=1"
		}

	filter "system:linux"
		defines{
			"CLV_PLATFORM_LINUX=1"
		}

	--Configuration filters
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"
		warnings "Extra"

		defines {
			"CLV_DEBUG=1"
		}
		
	filter "configurations:Development"
		runtime "Debug"
		symbols "On"
		optimize "Debug"
		warnings "Default"

		defines{
			"CLV_DEVELOPMENT=1"
		}
		
	filter "configurations:Profiling"
		runtime "Release"
		symbols "Off"
		optimize "On"
		warnings "Off"

		defines{
			"CLV_PROFILING=1"
		}

	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "Full"
		warnings "Off"

		defines{
			"CLV_RELEASE=1"
		}
	
	filter ""

--Clove Dependencies
group "Dependencies"
--GLAD
project "Glad"
	location "Clove/vendor/Glad"
    kind "StaticLib"
    language "C"
    
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
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

project "PortAudio"
	location "Clove/vendor/portaudio"
	kind "StaticLib"
	language "C"

	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	--converted from CMakeLists.txt in "Clove/vendor/portaudio"
	files{
		"%{prj.location}/include/portaudio.h",

		"%{prj.location}/src/common/pa_allocation.h",
		"%{prj.location}/src/common/pa_allocation.c",
  		"%{prj.location}/src/common/pa_converters.h",
		"%{prj.location}/src/common/pa_converters.c",
  		"%{prj.location}/src/common/pa_cpuload.h",
		"%{prj.location}/src/common/pa_cpuload.c",
  		"%{prj.location}/src/common/pa_debugprint.h",
		"%{prj.location}/src/common/pa_debugprint.c",
  		"%{prj.location}/src/common/pa_dither.h",
		"%{prj.location}/src/common/pa_dither.c",
		"%{prj.location}/src/common/pa_process.h",
		"%{prj.location}/src/common/pa_process.c",
		"%{prj.location}/src/common/pa_ringbuffer.h",
		"%{prj.location}/src/common/pa_ringbuffer.c",
		"%{prj.location}/src/common/pa_stream.h",
		"%{prj.location}/src/common/pa_stream.c",
		"%{prj.location}/src/common/pa_trace.h",
		"%{prj.location}/src/common/pa_trace.c",
		
		"%{prj.location}/src/common/pa_endianness.h",
		"%{prj.location}/src/common/pa_hostapi.h",
		"%{prj.location}/src/common/pa_memorybarrier.h",
  		"%{prj.location}/src/common/pa_types.h",
		"%{prj.location}/src/common/pa_util.h",
		  
		"%{prj.location}/src/common/pa_front.c",
	}

	includedirs{
		"%{prj.location}/include/",
		"%{prj.location}/src/common",
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
	}

	filter "system:windows"
		files{
			"%{prj.location}/include/pa_win_ds.h",
			"%{prj.location}/include/pa_win_waveformat.h",

			"%{prj.location}/src/os/win/pa_win_wdmks_utils.h",
			"%{prj.location}/src/os/win/pa_win_wdmks_utils.c",
			"%{prj.location}/src/os/win/pa_win_coinitialize.h",
			"%{prj.location}/src/os/win/pa_win_coinitialize.c",
			"%{prj.location}/src/hostapi/dsound/pa_win_ds_dynlink.h",
			"%{prj.location}/src/hostapi/dsound/pa_win_ds_dynlink.c",

			"%{prj.location}/src/os/win/pa_win_hostapis.c",
			"%{prj.location}/src/os/win/pa_win_waveformat.c",
			"%{prj.location}/src/os/win/pa_win_util.c",
			"%{prj.location}/src/hostapi/dsound/pa_win_ds.c"
		}

		includedirs{
			"%{prj.location}/src/os/win"
		}

		links{
			"dsound.dll",
		}

		defines{
			"PA_USE_DS",
			"PAWIN_USE_DIRECTSOUNDFULLDUPLEXCREATE"
		}
	
	filter "system:linux"
		files{
			"%{prj.location}/src/os/unix/pa_unix_hostapis.c",
			"%{prj.location}/src/os/unix/pa_unix_util.c",
		}
--End: Dependencies

--CLOVE
-- Inlcude direction relative to the roof folder (solution directory)
includeDir = {}
includeDir["Glad"]		= "Clove/vendor/Glad/include"
includeDir["glm"]		= "Clove/vendor/glm"
includeDir["stb"]		= "Clove/vendor/stb"
includeDir["dxerr"]		= "Clove/vendor/dxerr"
includeDir["portaudio"] = "Clove/vendor/portaudio/include"

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
		"%{prj.location}/src/**.hpp",
		"%{prj.location}/src/**.inl",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.glsl",
	}

	includedirs{
		--Clove
		"%{prj.location}/src",
		
		--Libs
		"%{includeDir.Glad}",
		"%{includeDir.glm}",
		"%{includeDir.stb}",
		"%{includeDir.dxerr}",
		"%{includeDir.portaudio}",

		--Misc vendor
		"%{prj.location}/vendor/spdlog/include",
		"%{prj.location}/vendor/OBJ-Loader/Source",
		"%{prj.location}/vendor/Event-Dispatcher",
	}

	links{
		"Glad",
		"stb",
		"portaudio"
	}

	defines{
		"CLV_ENGINE=1"
	}

	--platform specifics
	filter "system:windows"
		files{
			"%{prj.location}/src/**.hlsl",
		}

		removefiles{
			"%{prj.location}/src/Platform/Linux/**.hpp",
			"%{prj.location}/src/Platform/Linux/**.inl",
			"%{prj.location}/src/Platform/Linux/**.cpp",
			
			"%{prj.location}/src/Graphics/OpenGL-4/GLXContext.*",
		}

		includedirs{
			"%{prj.location}/vendor/wglext",
		}

		links{
			"dxerr",
			"opengl32.lib",
			"d3d11.lib",
			"d3dcompiler.lib",
		}

		postbuildcommands{
			("{COPY} "..hlslout_dir.."**.cso \"../Built/bin/"..outputdir.."/Sandbox/\"")
		}

	filter "system:linux"
		removefiles{
			"%{prj.location}/src/Platform/Windows/**.hpp",
			"%{prj.location}/src/Platform/Windows/**.inl",
			"%{prj.location}/src/Platform/Windows/**.cpp",

			"%{prj.location}/src/Graphics/DirectX-11/**.hpp",
			"%{prj.location}/src/Graphics/DirectX-11/**.inl",
			"%{prj.location}/src/Graphics/DirectX-11/**.cpp",

			"%{prj.location}/src/Graphics/OpenGL-4/WGLContext.*",
		}

		links{
			"GLX",
			"X11"
		}

	--hlsl shaders
	filter "files:**.hlsl"
		shadermodel "5.0"
		shaderobjectfileoutput(hlslout_com)
	filter "files:**-vs.hlsl"
		shadertype "Vertex"
	filter "files:**-ps.hlsl"
		shadertype "Pixel"

	filter ""

--Sandbox
group ""
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir(targetdir_clv)
	objdir(objdir_clv)

	files{
		"%{prj.location}/src/**.hpp",
		"%{prj.location}/src/**.inl",
		"%{prj.location}/src/**.cpp"
	}

	includedirs{
		"Clove/vendor/spdlog/include",

		"%{includeDir.Glad}",
		"%{includeDir.glm}",

		"Clove/src"
	}

	links{
		"Clove",
	}

	--gmake doesn't unpack static dependencies the same way msvc does
	filter "action:gmake2"
		links{
			"Glad",
			"GLX",
			"X11",
			"stb",
			"dl"
		}